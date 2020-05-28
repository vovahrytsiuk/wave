//
//  main.cpp
//  lab4
//
//  Copyright © 2020 Volodymyr Hrytsiuk. All rights reserved.
//

#include <iostream>
#include "bmp.h"

int main(int argc, char* argv[]) {
    if(argc != 4){
        std::cout << "Incorrect data" << std::endl;
        return 0;
    }
    BMPreader image(argv[1]);
    cout << "Resizing image " << atof(argv[3]) << "times\n";
    BMPreader newimage = image.resize(atof(argv[3]));
    newimage.write(argv[2]);
    cout << "Done\nResult written to " << argv[2] << endl;
    return 0;
}