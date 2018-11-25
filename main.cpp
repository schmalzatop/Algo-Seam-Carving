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
        
        //read in values
        for(int row = 0; row < w; row++)
        {
            for(int col = 0; col < h; col++)
            {
                ss >> arr[row][col];
            }
        }

        for(int i = 0; i < w; i++)
        {
            for(int o = 0; o < h; o++)
            {
                imgarr[i][o] = arr[i][o];
            }
        }

        image.close();
    }
    //Class Functions
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
    //create energy matrix
    void energy()
    {
        
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
            
        }
        else
        {
            //FIX-ME    add support for color images
            //toPGM(imgFile);   //take imgFile and then convert to .pgm save as imgFileNew.pgm
            //process(imgFileNEW);
        }
    }
	return 0;
}