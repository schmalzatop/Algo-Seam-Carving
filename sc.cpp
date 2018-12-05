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
  int ow;    //orginal width
  int oh;    //orginal height
  int w;   //current width
  int h;   //current height
  int ** imgarr;  //2d array for img
  int ** EP;   //2d array for energy
  int ** LCE; //2d array for LCE
  std::string gs; //hold Gray scale value

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
      if(input[0] == '#') //ignore the comment if there
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
          int wh = input.find(' ');
          ow = atoi((input.substr(0, wh)).c_str()); //get width
          oh = atoi((input.substr(wh + 1)).c_str()); //get height
          //create 2d array
          imgarr = new int * [oh];
          for(int i = 0; i < oh; i++)
          {
              imgarr[i] = new int[ow];
          }
          index++;
          break;
        }
        case 3: //grab the grey scale value '255'
        {
          gs = input;
          index++;
          break;
        }
        default: //record the pixel numbers
        {
          std::string num = "";
          int ls = input.size();
          for(int i = 0; i < ls; i++)
          {
              if((input[i] == ' ') || (input[i] == '\n') || (input[i] == '\t'))
              {
                  content.push_back(num);
                  num.clear();
              }
              else
              {
                  num += input[i];
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
    for(int y = 0; y < oh; y++)
    {
      for(int x = 0; x < ow; x++)
      {
        stuff = content[spot];
        imgarr[y][x] = atoi(stuff.c_str());
        spot++;
      }
    }

    EP = NULL;
    LCE = NULL;
    w = ow;
    h = oh;
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
        {
          left = imgarr[y][x];
        }
        else                            //LEFT: general case
        {
          left = imgarr[y][x - 1];
        }
        if(x == (w - 1))                //RIGHT: boundry case
        {
          right = imgarr[y][x];
        }
        else                            //RIGHT: general case
        {
          right = imgarr[y][x + 1];
        }
        if(y == 0)                      //UP: boundry case
        {
          up = imgarr[y][x];
        }
        else                            //UP: general case
        {
          up = imgarr[y - 1][x];
        }
        if(y == (h - 1))                //DOWN: boundry case
        {
          down = imgarr[y][x];
        }
        else                            //DOWN: general case
        {
          down = imgarr[y + 1][x];
        }

        EP[y][x] = abs(pixel - left) + abs(pixel - right) + abs(pixel - up) + abs(pixel - down);
      }
    }
  }

  void vCarve()
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
        {
          LCE[y][x] = EP[y][x];
        }
        else
        {
          int val;
          if(x == 0) //cases: when on left column cant use left
          {
            val = std::min(LCE[y - 1][x + 1], LCE[y - 1][x]);
          }
          else if(x == (w - 1)) //cases: when on right column cant use right
          {
            val = std::min(LCE[y - 1][x], LCE[y - 1][x - 1]); \
          }
          else //all cases: left, up, right
          {
            val = std::min(std::min(LCE[y - 1][x - 1], LCE[y -1][x]), LCE[y - 1][x + 1]);
          }
          LCE[y][x] = EP[y][x] + val;
        }
      }
    }

    int start = 0;
    for(int x = 0; x < w; x++) //find the smallest number in the bottom row
    {
      if(LCE[h - 1][x] < LCE[h - 1][start])
      {
        start = x;
      }
    }
    imgarr[h - 1][start] = -1; //set the smallest value to -1 - to track the seam

    for(int x = 1; x < h; x++)
    {
      int height = h - 1 - x; //track the row from bottom up
      int ls = start - 1;      //track the left col
      int as = start;          //track the above col
      int rs = start + 1;      //tracl the right col

      if(start == 0)   //handle the case where there is no valid left
      {
        ls = start;
      }
      if(start == (w - 1)) //handle the case where there is no valid right
      {
        rs = start;
      }

      //find the lowest value to go to
      int go = std::min(std::min(LCE[height][ls], LCE[height][as]), LCE[height][rs]);
      if(LCE[height][ls] == go)
      {
        start = ls;
      }
      else if(LCE[height][as] == go)
      {
        start = as;
      }
      else if(LCE[height][rs] == go)
      {
        start = rs;
      }

      imgarr[height][start] = -1;
    }

    //move the -1's to the right
    for(int y = 0; y < h; y++)
    {
      for(int x = 0; x < w - 1; x++)
      {
        if(imgarr[y][x] == -1)
        {
          int temp = imgarr[y][x];
          imgarr[y][x] = imgarr[y][x + 1];
          imgarr[y][x + 1] = temp;
        }
      }
    }
    w = w - 1;
  }

  void hCarve()
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
        {
          LCE[y][x] = EP[y][x];
        }
        else
        {
          int val;
          if(y == 0) //cases: when on top row cant use left and up
          {
            val = std::min(LCE[y][x - 1], LCE[y + 1][x - 1]);
          }
          else if(y == (h - 1)) //cases: when on bottom row cant use left and down
          {
            val = std::min(LCE[y][x - 1], LCE[y - 1][x - 1]);
          }
          else //all cases: left, up, right
          {
            val = std::min(std::min(LCE[y - 1][x - 1], LCE[y][x - 1]), LCE[y + 1][x - 1]);
          }
          LCE[y][x] = EP[y][x] + val;
        }
      }
    }

    int start = 0;
    for(int y = h -1; y > -1; y--) //find the smallest number in the right column
    {
      if(LCE[y][w -1] < LCE[start][w - 1])
      {
        start = y;
      }
    }
    imgarr[start][w - 1] = -1; //set the smallest value to -1 - to track the seam

    int shift = 1;
    for(int x = w - 1; x > -1; x--)
    {
      int height = w - 1 - shift; //track the row from right to left
      int lu = start - 1;      //track the left up
      int ls = start;          //track the left side
      int ld = start + 1;      //track the left down

      if(start == 0)   //handle the case where there is no valid left up
      {
        lu = start;
      }
      if(start == (h - 1)) //handle the case where there is no valid left down
      {
        ld = start;
      }

      //find the lowest value to go to
      int go = std::min(std::min(LCE[lu][height], LCE[ls][height]), LCE[ld][height]);
      if(LCE[lu][height] == go)
      {
        start = lu;
      }
      else if(LCE[ls][height] == go)
      {
        start = ls;
      }
      else if(LCE[ld][height] == go)
      {
        start = ld;
      }

      imgarr[start][height] = -1;
      shift++;
    }

    //move the -1's to the bottom
    for(int y = 0; y < h - 1; y++)
    {
      for(int x = 0; x < w; x++)
      {
        if(imgarr[y][x] == -1)
        {
          int temp = imgarr[y][x];
          imgarr[y][x] = imgarr[y + 1][x];
          imgarr[y + 1][x] = temp;
        }
      }
    }
    h = h - 1;
  }

  void shrink(int vSeams, int hSeams)
  {
    for(int y = 0; y < vSeams; y++) //cut the vertical seams
    {
      vCarve();
      energy();
    }

    for(int x = 0; x < hSeams; x++) //cut the horizontal seams
    {
      hCarve();
      energy();
    }
  }

  //Produces the correct output file
  void processed(std::string file)
  {
    int end = file.find_last_of(".");
    std::string output = file.substr(0, end);
    output += "_processed.pgm";
    std::ofstream processedOut(output);
    processedOut << "P2" << '\n' << w << ' ' << h << '\n' << gs << '\n';
    for(int y = 0; y < h; y++)
    {
      for (int x = 0; x < w; x++)
      {
        processedOut << imgarr[y][x] << ' ';
      }
    }
    processedOut.close();
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

  //print master imgarr array
  void printMaster()
  {
    std::cout << "Master" << std::endl;
    std::cout << "orginal w: " << ow << " | orginal h: " << oh << std::endl;
    for(int row = 0; row < oh; row++)
    {
      for(int col = 0; col < ow; col++)
      {
        //std::cout << *(imgarr + row * h + col);
        int num = imgarr[row][col];
        std::cout << num << " | ";
      }
      std::cout << "" << std::endl;
    }
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
        i.shrink(vSeams, hSeams);
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
