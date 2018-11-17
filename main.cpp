// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h> 
#include <fstream>



int main(int argc, char *argv[])
{
    if (argc < 3) 
        std::cout << "wrong format! should be 'a.exe Image  Vertical_Size  Horizontal_Size'" << std::endl;
    else 
    { 
        std::string imgFile = argv[1];
        std::string vSize = argv[2];
        std::string hSize = argv[3];

        //verify the read in of the image
        std::ifstream img(imgFile);
        if(!img.is_open())
        {
            std::cout << "The image could not be opened" << std::endl;
        }

        //convert image to .pgm



        //determine if an enlarge or a shrink
        


        img.close();
    }

	return 0;
}