// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h> 


int main(int argc, char *argv[])
{
    if (argc < 3) 
        std::cout << "wrong format! should be 'a.exe image.pgm  Vertical_Size  Horizontal_Size'" << std::endl;
    else 
    { 
        std::string img = argv[1];
        std::string vSize = argv[2];
        std::string hSize = argv[3];
        
   }

	return 0;
}