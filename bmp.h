//
//  bmp.hpp
//  lab4
//
//  Created by Volodymyr on 5/21/20.
//  Copyright © 2020 Volodymyr Hrytsiuk. All rights reserved.
//

#ifndef bmp_hpp
#define bmp_hpp

#include <stdio.h>
#include <stdint.h>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

typedef struct {
    uint32_t size={ 40L };
    int32_t width={ 0 };
    int32_t depth={ 0 };
    uint16_t biPlanes={ 1 };
    uint16_t bits={ 24 };
    uint32_t biCompression={ 0L };
    uint32_t biSizeImage={ 0L };
    int32_t biXPelsPerMeter={ 0L };
    int32_t biYPelsPerMeter={ 0L };
    uint32_t biClrUsed={ 0L };
    uint32_t biClrImportant={ 0L };
} BMPInfoHeader;

#pragma pack(push, 1)
typedef struct {
    uint8_t id[2];
    uint32_t filesize={ 0 };
    uint16_t reserved[2]={ 0 , 0};
    uint32_t headersize={ 54L };
} BMPFileHeader;
#pragma pack(pop)

typedef struct {
    uint8_t r={ 0 };
    uint8_t g={ 0 };
    uint8_t b={ 0 };
} Pixel;


class BMPreader{
    BMPInfoHeader info_header;
    BMPFileHeader header;
    vector<vector<Pixel> > pixel_data;
    int32_t padding = 0;
    void caclulate_size();
    void resample(vector<vector<Pixel> > start, vector<vector<Pixel> >&result);
    
public:
    BMPreader(const char* file_name);
    void read(const char* file_name);
    void write(const char* file_name);
    BMPreader resize(float coef);
    BMPreader();
    BMPreader(uint32_t depth, uint32_t width);
};



#endif /* bmp_hpp */
