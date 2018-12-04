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
        std::string input;
        std::vector<std::string> content;
        int index = 1;

        while(getline(image, input))
        {
            if(line[0] == '#') //ignore the comment if there
            {
                continue;
            }

            switch(index)
            {
                case 1: 
                {
                    index++;
                    break;
                }
                case 2: //get the width and height
                {
                    int wh = line.find(' ');
                    w = atoi((line.substr(0, wh)).c_str()); //get width
                    h = atoi((line.substr(wh + 1)).c_tr()); //get height
                    //create 2d array
                    imgarr = new int * [h];
                    for(int i = 0; i < h; i++)
                    {
                        imgarr[i] = new int[w];
                    }
                    index++;
                    break;
                }
                case 3: //grab the grey scale value '255'
                {
                    gs = line;
                    index++;
                    break;
                }
                default: //record the pixel numbers
                {
                    std::string num = "";
                    int ls = line.size();
                    for(int i = 0; i < ls; i++)
                    {
                        if((line[i] == ' ') || (line[i] == '\n') || (line[i] == '\t'))
                        {
                            content.push_back(num);
                            n.clear();
                        }
                        else
                        {
                            num += line[i];
                        }
                    }
                    if(!num.empty())
                    {
                        content.push_back(num);
                    }
                    break;
                }
            }
        }
        image.close();
        //add the pixel values to the 2d imgarr array
        int spot = 0;
        std::string stuff;
        for(int y = 0; y < h; y++)
        {
            for(int x = 0; x < w; x++)
            {
                stuff = content[spot];
                imgarr[i][j] = atoi(stuff.c_str());
                spot++;
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

    void shrink(int vSeams, int hSeams)
    {
        for(int y = 0; y < vSeams; y++) //cut the vertical seams
        {

        }

        for(int x = 0; x < hSeams; x++) //cut the horizontal seams
        {

        }
    }
}

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