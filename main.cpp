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
    int** LCE;
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
                //std::cout << spot << " | " << imgarr[i][j] << " j: " << j << " i: " << i << std::endl;
            }
        }
        EP = NULL;
        LCE = NULL;
        energy();
    }

    //Class Functions

    //create energy array
    void energy()
    {
        if(EP != NULL) 
        {
        for(int i = 0; i < (w - 1); i++)
        {
            delete [] EP[i];
        }
        delete [] EP;
        }

        //create energy array
        EP = new int * [h];
        for(int i = 0; i < h; i++)
        {
            EP[i] = new int[w];
        }

        //find energy values for each pixel
        for(int y = 0; y < h; y++)
        {
            for(int x = 0; x < w; x++)
            {
                int pixel = imgarr[y][x];   //pixel in question
                int u, d, l, r;     //u - up; d - down; l - left; r - right;
                //handle left side case
                if(x == 0)
                {
                    l = imgarr[y][x];
                }
                else
                {
                    l = imgarr[y][x - 1];
                }
                //handle right side case
                if(x == (w-1))
                {
                    r = imgarr[y][x];
                }
                else
                {
                    r = imgarr[y][x + 1];
                }
                //handle top case
                if(y == 0)
                {
                    u = imgarr[y][x];
                }
                else
                {
                    u = imgarr[y - 1][x];
                }
                //handle bottom case
                if(y == (h - 1))
                {
                    d = imgarr[y][x];
                }
                else
                {
                    d = imgarr[y + 1][x];
                }
                
                //calc energy lvl and insert
                EP[y][x] = (abs(pixel - l)+abs(pixel - r)+abs(pixel - u)+abs(pixel - d));
                //std::cout << abs(pixel - l) << " + " << abs(pixel - r) << " + " << abs(pixel - u) << " + " << abs(pixel - d) << " = " << std::endl;
            }
        } 
    }

    //print the 2d array - imgarr[vertical - h ][horizaotnal - w]
    void printArr()
    {
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

    //creates the Least Cumlation Energy Array
    void larr()
    {
        //create the Least Cummlation Energy Array
        //int** LCE;
        LCE = new int * [h];
        for(int i = 0; i < h; i++)
        {
            LCE[i] = new int[w];
        }
        //fill the LCE Array  - if above row has equal options will choose leftmost option
        for(int y = 0; y < h; y++)
        {
            for(int x = 0; x < w; x++)
            {
                if(y == 0) //top row condition
                {
                    LCE[y][x] = EP[y][x];
                }
                else if(x == 0) //left most condtion 
                {
                    if(LCE[y - 1][x + 1] < LCE[y - 1][x]) //right < above
                    {
                        LCE[y][x] = LCE[y - 1][x + 1] + EP[y][x];
                    }
                    else //above < right
                    {
                        LCE[y][x] = LCE[y - 1][x] + EP[y][x];
                    }
                }
                else if(x == (h - 1)) //right most condition
                {
                    if(LCE[y - 1][x] < LCE[y - 1][x - 1]) //above < left
                    {
                        LCE[y][x] = LCE[y - 1][x] + EP[y][x];
                    }
                    else //left < above
                    {
                        LCE[y][x] = LCE[y - 1][x - 1] + EP[y][x];
                    }
                }
                else //compare right, above, & left condition
                {
                    int smallest = LCE[y - 1][x - 1];
                    if (smallest > LCE[y - 1][x]) //if left is bigger than above set to above
                    {
                        smallest = LCE[y - 1][x];
                    }
                    if(smallest > LCE[y - 1][x + 1]) // left or above is bigger than right set to right
                    {
                        smallest = LCE[y - 1][x + 1];
                    }
                    LCE[y][x] = smallest + EP[y][x];
                }
                //std::cout << "Row: " << y << "Col: " << x << std::endl;
            }
        }
    }

    void orient()
    {
        //flip the values
        int ** arrimg = imgarr;
        int x = w;
        int y = h;
        h = x;
        w = y;
        
        //rebuild
        imgarr = new int * [h];
        for(int i = 0; i < h; i++)
        {
            imgarr[i] = new int[w];
        }

        for(int k = 0; k < h; k++)
        {
            for(int j = 0; j < w; j++)
            {
                imgarr[k][j] = arrimg[w - 1 - j][k];
            }
        }

        energy();
        
        //clean up
        for(int d = 0; d < h; d++)
        {
            delete [] arrimg[d];
        }
        delete [] arrimg;
    }

    void carve()
    {
        int min = 0;
        for(int i = 0; i < w; i++)  //find the col with the smallest value in bottom row - if tie leftmost is chosen
        {
            if(LCE[h - 1][i] < LCE[h - 1][min])
            {
                min = i;
            }
        }
        imgarr[h - 1][min] = -1;      //mark spots to remove with -1
        int ncol = min;
        for(int c = 0; c < h; c++)
        {
            int lvl = h - 1 - c;
            int above = ncol;
            int left = ncol - 1;
            int right = ncol + 1;

            //handle the edge cases
            if(left <= 0)
            {
                left = above;
            }
            if(right >= h - 1)
            {
                right = above;
            }

            //pick the next position
            int smallest = LCE[lvl][left];
            if(smallest > LCE[lvl][above]) //if left is bigger than above set to above
            {
                smallest = LCE[lvl][above];
            }
            if(smallest > LCE[lvl][right]) //if left or above is bigger then right set to right
            {
                smallest = LCE[lvl][right];
            }
            if(smallest == LCE[lvl][left])
            {
                ncol = left;
            }
            else if(smallest == LCE[lvl][above])
            {
                ncol = above;
            }
            else
            {
                ncol = right;
            }

            imgarr[lvl][ncol] = -1;
        }
        std::vector<int> temp;
        int f = w;
        --w;
        printArr();
        for(int j = 0; j < h; j++)
        {
            for(int k = 0; k < f; k++)
            {
                if(imgarr[j][k] != -1)
                {
                    temp.push_back(imgarr[j][k]);
                }
            }
        }
        
        //delete the orginal array to remove the -1 and shrink
        for(int d = 0; d < w; d++)
        {
            delete [] imgarr[d];
        }
        delete [] imgarr;

        //recreate the orginal array at smaller size
        imgarr = new int * [h];
        for(int i = 0; i < h; i++)
        {
            imgarr[i] = new int[w];
        }

        //fill the array
        int spot = 0;
        for(int i = 0; i < h; i++)
        {
            for(int o = 0; o < w; o++)
            {
                imgarr[i][o] = temp[spot];
                spot++;
            }
        }
        temp.clear();
    }

    void shrink(int hor, int vert)
    {
        //carve the vertical seams
        for(int v = 0; v < vert; v++)
        {
            larr();
            carve();
            energy();  
        }
        
        orient();
    
        //carve the horizontal seams
        for(int h = 0; h < hor; h++)
        {
            larr();
            carve();
            printArr();
            //energy();
        }
    }

    void enlarge(int hor, int vert)
    {
        std::cout << "Feature to be Added" << std::endl;
        //create the Least Cummlation Energy Array
        larr();
        //Find seam - while loop for amount of seams - Horizontal - Add Seams

        //Find seam - while loop for amount of seams - Vertical - Add Seams
    }

    void remove()
    {
        std::cout << "Feature to be Added" << std::endl;
    }

    //writes processed img to file with modified name
    void processed(std::string name) 
    {
        int here = name.find_last_of('.');
        std::string output = name.substr(0, here);
        output += "_processed.pgm";
        std::ofstream file(output);
        file << "P2" << '\n' << w << ' ' << h << '\n' << gs << '\n';
        for(int i = 0; i < h; i++) 
        {
            for (int j = 0; j < w; j++) 
            {
                file << imgarr[j][i] << ' ';
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
            //i.printArr();
            //i.printEArr();
            if(chng == "S")
            {
                i.shrink(hSeams, vSeams);
                //i.printArr();
                //i.printEArr();
                //i.printLCEArr();
                //i.processed(imgFile);
            }
            else if(chng == "E")
            {
                //FIX-ME    add support to enlarge images
                i.enlarge(hSeams, vSeams);
                //i.processed();
            }
            else if(chng == "R")
            {
                //FIX-ME    add suuport to remove items
                i.remove();
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