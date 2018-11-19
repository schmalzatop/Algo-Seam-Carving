//Spencer Schmalz | scs102@zips.uakron.edu
//Alogirthms Project 3 Seam Carving

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h> 
#include <fstream>
#include <stdio.h>

//function to read in .pgm file into a 2d array
void readPGM(std::ifstream& img, int arr[])
{
    
}

//function to drive the conversion from color to grey and to create a 2d array
void toGrey(int *arry[], int *gryImg[])
{
    
}

//function to drive the carving of the image
void shrink(int *img, std::string VSeams, std::string HSeams)
{
    
}

//function to drive the 'adding' to the image
void enlarge(int *img, std::string VSeams, std::string HSeams)
{

}

//function to remove an object from the image
void remove(int *img, std::string VSeams, std::string HSeams)
{

}

int main(int argc, char *argv[])
{
    if (argc < 4) 
        std::cout << "wrong format! should be 'a.exe Image  Vertical_Size Horizontal_Size' 'Shrink (S), Enlarge (E), Remove Obeject (R)'" << std::endl;
    else 
    { 
    //set inputs
        std::string imgFile = argv[1];
        std::string vSeams = argv[2];
        std::string hSeams = argv[3];
        std::string chng = argv[4];

    //make sure image is able to be opened
        std::ifstream image;
        image.open(imgFile);
        if(!image.is_open())
        {
            std::cout << "The image could not be opened" << std::endl;
            return 0;
        }
    
    //check the file type
        int asize = 0;
        int *gryP;
        if(imgFile.substr(imgFile.find_last_of(".") + 1) == "pgm")
        {
            std::string line;
            while(getline(image, line))
            {
                asize++;
            }
            int ImgArr [asize];  //array to hold the 2d image
            readPGM(image, ImgArr);
            gryP = ImgArr;
        }
        else //handle other files types i.e. .jgp .png etc...
        {
//FIX-ME    create function to convert read in file to .pgm  
        }

        image.close();

    //enlarge or shrink
        if (chng == "S")
        {
            shrink(gryP, vSeams, hSeams);
        }
        else if (chng == "R")
        {
            enlarge(gryP, vSeams, hSeams);
        }
        else
        {
            remove(gryP, vSeams, hSeams);
        }
    }
	return 0;
}