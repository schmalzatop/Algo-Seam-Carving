//Spencer Schmalz | scs102@zips.uakron.edu
//Alogirthms Project 3 Seam Carving

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h> 
#include <fstream>
#include <sstream>

struct image
{
    //Member Variables
    int w;
    int h;
    int ** imgarr;
    int ** EP;
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
        imgarr = new int * [w];
        for(int i = 0; i < w; i++)
        {
            imgarr[i] = new int[h];
        }
        //create array
        int arr[w][h];

        ss >> gs;
        //std::cout << "GS: " << gs << std::endl;

        //read in values
        for(int row = 0; row < w; row++)
        {
            for(int col = 0; col < h; col++)
            {
                ss >> arr[row][col];
            }
        }

        //fill the array
        for(int i = 0; i < w; i++)
        {
            for(int o = 0; o < h; o++)
            {
                imgarr[i][o] = arr[i][o];
            }
        }

        image.close();

        EP = NULL;
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
        EP = new int * [w];
        for(int i = 0; i < w; i++)
        {
            EP[i] = new int[h];
        }

        //find energy values for each pixel
        for(int y = 0; y < h; y++)
        {
            for(int x = 0; x < w; x++)
            {
                int pixel = imgarr[x][y];   //pixel in question
                int u, d, l, r;     //u - up; d - down; l - left; r - right;
                //handle left side case
                if(x == 0)
                {
                    l = imgarr[x][y];
                }
                else
                {
                    l = imgarr[x - 1][y];
                }
                //handle right side case
                if(x == (w-1))
                {
                    r = imgarr[x][y];
                }
                else
                {
                    r = imgarr[x + 1][y];
                }
                //handle top case
                if(y == 0)
                {
                    u = imgarr[x][y];
                }
                else
                {
                    u = imgarr[x][y - 1];
                }
                //handle bottom case
                if(y == (h - 1))
                {
                    d = imgarr[x][y];
                }
                else
                {
                    d = imgarr[x][y + 1];
                }
                
                //calc energy lvl and insert
                EP[x][y] = (abs(pixel - l)+abs(pixel - r)+abs(pixel - u)+abs(pixel - d));
            }
        } 
    }

    //print the 2d array
    void printArr()
    {
        std::cout << "w: " << w << " | h: " << h << std::endl;
        for(int row = 0; row < w; row++)
        {
            for(int col = 0; col < h; col++)
            {
                //std::cout << *(imgarr + row * h + col);
                int num = imgarr[row][col];
                std::cout << num << " ";
            }
            std::cout << "" << std::endl;
        }
    }

    //print the energy lvl of the 2d array
    void printEArr()
    {
        std::cout << "w: " << w << " | h: " << h << std::endl;
        for(int row = 0; row < w; row++)
        {
            for(int col = 0; col < h; col++)
            {
                //std::cout << *(imgarr + row * h + col);
                int num = EP[row][col];
                std::cout << num << " ";
            }
            std::cout << "" << std::endl;
        }
    }

    void shrink(int hor, int vert)
    {
        std::cout << "Horizontal: " << hor << " | Vertical: " << vert << std::endl;
        //create the Least Cummlation Energy Array
        int** LCE;
        LCE = new int * [w];
        for(int i = 0; i < w; i++)
        {
            LCE[i] = new int[h];
        }
        //fill the LCE Array
        

        //Find seam - while loop for amount of seams - Horizontal - Remove Seams

        //Find seam - while loop for amount of seams - Vertical - Remove Seams

    }

    void enlarge(int hor, int vert)
    {
        std::cout << "Feature to be Added" << std::endl;
        //create the Least Cummlation Energy Array

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
                //i.processed();
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