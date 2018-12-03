//Spencer Schmalz | scs102@zips.uakron.edu
//Alogirthms Project 3 Seam Carving

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h> 
#include <fstream>
#include <sstream>
#include <vector>

struct image
{
    //Member Variables
    int w;
    int h;
    int ** imgarr;
    int ** EP;
    int ** LCE;
    std::string gs;

    //Deafult Constructor
    image() {};

    //File Name Constructor
    image(std::string file)
    {
        std::ifstream image;
        image.open(file);
        std::stringstream ss;
        std::string input = "";
        //check for 'P2'
        getline(image, input);
        if(input.compare("P2") != 0)
        {
            std::cerr << "Version Error: Not 'P2'" << std::endl;
        }
        
        //ignore comment
        getline(image, input);
        
        //get size
        ss << image.rdbuf();
        ss >> w >> h;
        imgarr = new int * [h];
        for(int i = 0; i < h; i++)
        {
            imgarr[i] = new int[w];
        }

        //create buffer vector
        std::vector<int> temp;

        ss >> gs;
        //std::cout << "GS: " << gs << std::endl;

        //read in values
        int hold;
        while(ss >> hold)
        {
            temp.push_back(hold);
        }

        image.close(); 

        //fill the array
        int spot = 0;
        int stuff; 
        for(int i = 0; i < h; i++)
        {
            for(int j = 0; j < w; j++)
            {
                stuff = temp[spot];
                imgarr[i][j] = stuff;
                ++spot;
            }
        }
        
        EP = NULL;
        LCE = NULL;
        energy();
    }

    //Class Functions
    void energy() //create energy array
    {
        if(EP != NULL) //if the array is already there destroy and re-make
        {
            for(int x = 0; x < w; x++)
            {
                delete [] EP[x];
            }
            delete [] EP;
        }

        EP = new int *[h]; //create the energy array
        for(int x = 0; x < h; x++)
        {
            EP[x] = new int[w];
        }

        for(int y = 0; y < h; y++)
        {
            for(int x = 0; x < w; x++)
            {
                int pixel = imgarr[y][x]; //pixel to work on
                int up, down, right, left; //surrounding pixels in question

                if(x == 0)                      //LEFT: boundry case 
                { left = imgarr[y][x]; }
                else                            //LEFT: general case
                { left = imgarr[y][x - 1]; }
                if(x == (w - 1))                //RIGHT: boundry case
                { right = imgarr[y][x]; }
                else                            //RIGHT: general case
                { right = imgarr[y][x + 1]; }
                if(y == 0)                      //UP: boundry case
                { up = imgarr[y][x]; }
                else                            //UP: general case
                { up = imgarr[y - 1][x]; }
                if(y == (h - 1))                //DOWN: boundry case
                { down = imgarr[y][x]; }
                else                            //DOWN: general case
                { down = imgarr[y + 1][x]; }

                EP[y][x] = abs(pixel - left) + abs(pixel - right) + abs(pixel - up) + abs(pixel - down);
            }
        }
    }

    void vertCarve() //creates the Least Cumlation Energy Array - for carving or adding
    {
        if(LCE != NULL) //if the array is already there destroy and re-make
        {
            for(int x = 0; x < w; x++)
            {
                delete [] LCE[x];
            }
            delete [] LCE;
        }

        LCE = new int *[h]; //create the energy array
        for(int x = 0; x < h; x++)
        {
            LCE[x] = new int[w];
        }

        for(int y = 0; y < h; y++)
        {
            for(int x = 0; x < w; x++)
            {
                if(y == 0) //base case - top row 
                { LCE[y][x] = EP[y][x]; }
                else
                {
                    int val;
                    if(x == 0) //cases: when on left column cant use left
                    { val = std::min(LCE[y - 1][x + 1], LCE[y - 1][x]); }
                    else if(x == (w - 1)) //cases: when on right column cant use right
                    { val = std::min(LCE[y - 1][x], LCE[y - 1][x - 1]); }
                    else //all cases: left, up, right
                    { val = std::min(std::min(LCE[y - 1][x - 1], LCE[y -1][x]), LCE[y - 1][x + 1]); }
                    LCE[y][x] = EP[y][x] + val;
                }
            }
        }

        int start = 0;
        for(int x = 0; x < w; x++) //find the smallest number in the bottom row
        {
            if(LCE[h - 1][x] < LCE[h - 1][start])
            { start = x; }
        }
        imgarr[h - 1][start] = -1; //set the smallest value to -1 - to track the seam
        
        for(int x = 1; x < h; x++)
        {
            int height = h - 1 - x; //track the row from bottom up
            int ls = start - 1;      //track the left col
            int as = start;          //track the above col
            int rs = start + 1;      //tracl the right col

            if(start == 0)   //handle the case where there is no valid left
            { ls = start; }
            if(start == (w - 1)) //handle the case where there is no valid right
            { rs = start; }
            
            //find the lowest value to go to
            int go = std::min(std::min(LCE[height][ls], LCE[height][as]), LCE[height][rs]);
            if(LCE[height][ls] == go)
            { start = ls; }
            else if(LCE[height][as] == go)
            { start = as; }
            else if(LCE[height][rs] == go)
            { start = rs; }

            imgarr[height][start] = -1;
        }
        
        //save the remaining parts of the image
        std::vector<int> save;
        for(int y = 0; y < h; y++)
        {
            for(int x = 0; x < w; x++)
            {
                if(imgarr[y][x] != -1)
                { save.push_back(imgarr[y][x]); }
            }
        }
        //printArr();
        //shrink
        int count = 0;
        //delete the image array 
        for(int x = 0; x < w; x++)
        {
            //std::cout << count << std::endl;
            delete [] imgarr[x];
            count++;
        }
        delete [] imgarr;
        
        w = w - 1;

        imgarr = new int * [h];
        for(int x = 0; x < h; x++)
        { imgarr[x] = new int[w]; }

        int hold = 0;
        for(int y = 0; y < h; y++)
        {
            for(int x = 0; x < w; x++)
            {
                imgarr[y][x] = save[hold];
                ++hold;
            }
        }
        save.clear();
    }

    void horiCarve() //creates the Least Cumlation Energy Array - for carving or adding
    {
        if(LCE != NULL) //if the array is already there destroy and re-make
        {
            for(int x = 0; x < w; x++)
            {
                delete [] LCE[x];
            }
            delete [] LCE;
        }

        LCE = new int *[h]; //create the energy array
        for(int x = 0; x < h; x++)
        {
            LCE[x] = new int[w];
        }
        
        for(int x = 0; x < w; x++)
        {
            for(int y = 0; y < h; y++)
            {
                if(x == 0) //base case - left column
                { LCE[y][x] = EP[y][x]; }
                else
                {
                    int val;
                    if(y == 0) //cases: when on top row cant use left and up
                    { val = std::min(LCE[y][x - 1], LCE[y + 1][x - 1]); }
                    else if(y == (h - 1)) //cases: when on bottom row cant use left and down
                    { val = std::min(LCE[y - 1][x - 1], LCE[y][x - 1]); }
                    else //all cases: up, side, down, (all left)
                    { val = std::min(std::min(LCE[y - 1][x - 1], LCE[y][x - 1]), LCE[y + 1][x - 1]); }
                    LCE[y][x] = EP[y][x] + val;
                }
            }
        }

        int start = 0;
        for(int x = 0; x < h; x++) //find the smallest number in the left column
        {
            if(LCE[x][w - 1] <= LCE[start][w - 1])
            { start = x; }
        }
        imgarr[start][w - 1] = -1; //set the smallest value to -1 - to track the seam

        for(int x = w-1; x > -1; x--)
        {
            int height = w - 2 - x; //track the col from right to left
            int lu = start - 1;      //track the left up
            int ss = start;          //track the left side
            int ld = start + 1;      //tracl the left down

            if(start == 0)   //handle the case where there is no valid left up
            { lu = start; }
            if(start == (h - 1)) //handle the case where there is no valid right up
            { ld = start; }
            
            //find the lowest value to go to
            int go = std::min(std::min(LCE[lu][height], LCE[ss][height]), LCE[ld][height]);
            if(LCE[ld][height] == go)
            { start = ld; }
            else if(LCE[ss][height] == go)
            { start = ss; }
            else if(LCE[lu][height] == go)
            { start = lu; }

            imgarr[start][height] = -1;
        }
        
        //save the remaining parts of the image
        std::vector<int> save;
        for(int y = 0; y < h; y++)
        {
            for(int x = 0; x < w; x++)
            {
                if(imgarr[y][x] != -1)
                { save.push_back(imgarr[y][x]); }
            }
        }
        
        //shrink
        int count = 0;
        //delete the image array 
        for(int x = 0; x < w; x++)
        {
            //std::cout << count << std::endl;
            delete [] imgarr[x];
            count++;
        }
        delete [] imgarr;
        
        h = h - 1;

        imgarr = new int * [h];
        for(int x = 0; x < h; x++)
        { imgarr[x] = new int[w]; }

        int hold = 0;
        for(int y = 0; y < h; y++)
        {
            for(int x = 0; x < w; x++)
            {
                imgarr[y][x] = save[hold];
                ++hold;
            }
        }
        save.clear(); 
    }

    void shrink(int hor, int vert)
    {
        //carve the vertical seams
        for(int v = 0; v < vert; v++)
        {
            vertCarve();
            energy();
        }

        //carve the horizontal seams
        for(int h = 0; h < hor; h++)
        {
            horiCarve();
            energy();
        }
    }

    //print the 2d array - imgarr[vertical - h ][horizaotnal - w]
    void printArr()
    {
        std::cout << "IMG" << std::endl;
        std::cout << "w: " << w << " | h: " << h << std::endl;
        for(int row = 0; row < h; row++)
        {
            for(int col = 0; col < w; col++)
            {
                //std::cout << *(imgarr + row * h + col);
                int num = imgarr[row][col];
                std::cout << num << " | ";
            }
            std::cout << "" << std::endl;
        }
    }

    //print the energy lvl of the 2d array
    void printEArr()
    {
        std::cout << "Energy" << std::endl;
        std::cout << "w: " << w << " | h: " << h << std::endl;
        for(int row = 0; row < h; row++)
        {
            for(int col = 0; col < w; col++)
            {
                //std::cout << *(imgarr + row * h + col);
                int num = EP[row][col];
                std::cout << num << " | ";
            }
            std::cout << "" << std::endl;
        }
    }

    //print the LCE of the 2d array
    void printLCEArr()
    {
        std::cout << "LCE" << std::endl;
        std::cout << "w: " << w << " | h: " << h << std::endl;
        for(int row = 0; row < h; row++)
        {
            for(int col = 0; col < w; col++)
            {
                //std::cout << *(imgarr + row * h + col);
                int num = LCE[row][col];
                std::cout << num << " | ";
            }
            std::cout << "" << std::endl;
        }
    }

    //Produces the correct output file
    void processed(std::string fileName)
    {
        int end = fileName.find_last_of(".");
        std::string output = fileName.substr(0, end);
        output += "_processed.pgm";
        std::ofstream file(output);
        file << "P2" << '\n' << w << ' ' << h << '\n' << gs << '\n';
        for(int i = 0; i < h; i++) 
        {
            for (int j = 0; j < w; j++) 
            { 
                file << imgarr[i][j] << ' ';
            }
        }
        file.close();
    } 
};

int main(int argc, char *argv[])
{
    if (argc < 4) 
        std::cout << "wrong format! should be 'a.exe Image  Vertical_Size Horizontal_Size' 'Shrink (S), Enlarge (E), Remove Obeject (R)'" << std::endl;
    else 
    { 
    //set inputs
        std::string imgFile = argv[1];
        int vSeams = atoi(argv[2]);
        int hSeams = atoi(argv[3]);
        std::string chng = argv[4];

    //check the file type
        if(imgFile.substr(imgFile.find_last_of(".") + 1) == "pgm")
        {
            image i(imgFile);
            if(chng == "S")
            {
                i.shrink(hSeams, vSeams);
                i.processed(imgFile);
            }
            else if(chng == "E")
            {
                //FIX-ME    add support to enlarge images
                //i.enlarge(hSeams, vSeams);
                //i.processed();
            }
            else if(chng == "R")
            {
                //FIX-ME    add suuport to remove items
                //i.remove();
                //i.processed();
            }
            else
            {
                std::cout << "Not a valid option!" << std::endl;
                return 0;
            }
        }
        else
        {
            //FIX-ME    add support for color images
            //toPGM(imgFile);   //take imgFile and then convert to .pgm save as imgFileNew.pgm
        }
    }
	return 0;
}