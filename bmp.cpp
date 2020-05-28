//
//  bmp.cpp
//  lab4
//
//  Created by Volodymyr on 5/21/20.
//  Copyright © 2020 Volodymyr Hrytsiuk. All rights reserved.
//

#include "bmp.h"


BMPreader::BMPreader(const char* file_name){
    read(file_name);
}

BMPreader::BMPreader(){};

BMPreader::BMPreader(uint32_t depth, uint32_t width){
    info_header.depth = depth;
    info_header.width = width;
    if(((info_header.width*3)%4)!=0){
        padding = 4-(info_header.width*3)%4;
    }
    caclulate_size();
    pixel_data.resize(info_header.depth);
    for(auto i = 0; i < info_header.depth; i++){
        pixel_data[i].resize(info_header.width);
    }
    header.filesize+=pixel_data.size()*(pixel_data[0].size()*3 + padding);
    
}

void BMPreader::read(const char* file_name){
    ifstream fin(file_name, ios_base::binary);
    fin.read((char*)&header, sizeof(header));
    fin.read((char*)&info_header, sizeof(info_header));
    Pixel rgb_lite;
    
    if(((info_header.width*3)%4)!=0){
        padding = 4-(info_header.width*3)%4;
    }
    caclulate_size();
    
    pixel_data.resize(info_header.depth);
    for(auto i = 0; i < info_header.depth; i++){
        pixel_data[i].resize(info_header.width);
    }
    for(auto i = 0; i < info_header.depth; i++){
        fin.read((char*)pixel_data[i].data(), info_header.width*sizeof(Pixel));
        for(auto i = 0; i < padding; i++){
            fin.read((char*)&rgb_lite,1);
        }
    }
    header.filesize+=pixel_data.size()*(pixel_data[0].size()*3 + padding);
    fin.close();
}

void BMPreader::caclulate_size(){
    info_header.size = sizeof(BMPInfoHeader);
    header.headersize= sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
    header.filesize = header.headersize;
}

void BMPreader::write(const char* file_name){
    Pixel rgb_lite{0,0,0};
    ofstream fout(file_name, ios_base::binary);
    fout.write((char*)&header, sizeof(header));
    fout.write((char*)&info_header, sizeof(info_header));
    for(auto i = 0; i < info_header.depth; i++){
        fout.write((char*)pixel_data[i].data(), info_header.width*sizeof(Pixel));
        for(auto j = 0; j < padding; j++){
            fout.write((char*)&rgb_lite, 1);
        }
    }
    fout.close();
}

BMPreader BMPreader::resize(float coef){
    BMPreader newimage;
    int32_t newdepth = ceil(this->info_header.depth*coef);
    int32_t newwidth = ceil(this->info_header.width*coef);
    newimage.header = this->header;
    newimage.info_header = this->info_header;
    newimage.info_header.width = newwidth;
    newimage.info_header.depth = newdepth;
    newimage.caclulate_size();
    if((newimage.info_header.width*3)%4!=0){
        newimage.padding=4- (newimage.info_header.width*3)%4;
    }
    newimage.header.filesize+=newdepth*(newwidth*3 + newimage.padding);
    newimage.pixel_data.resize(newimage.info_header.depth);
    for(auto i = 0; i < newimage.info_header.depth; i++){
        newimage.pixel_data[i].resize(newimage.info_header.width);
    }
    
    resample(this->pixel_data, newimage.pixel_data);
    return newimage;
}

void BMPreader::resample(vector<vector<Pixel> > start, vector<vector<Pixel>>&result){
    auto newwidth = result[0].size();
    auto newdepth = result.size();
    auto oldwidth = start[0].size();
    auto olddepth = start.size();
    Pixel p1, p2, p3, p4, temp;
    float x_r = (float)(oldwidth-1)/newwidth;
    float y_r = (float)(olddepth-1)/newdepth;
    for(auto i = 0; i < newdepth; i++){
        for(auto j = 0; j < newwidth; j++){
            int32_t x = (int)(x_r*i);
            int32_t y = (int)(y_r*j);
            float dx = x_r*i - x;
            float dy = y_r*j - y;
            float d1 = (1-dx)*(1-dy);
            float d2 = (1-dx)*dy;
            float d3 = dx*dy;
            float d4 = dx*(1-dy);
            p1 = start[x][y];
            p2 = start[x][y+1];
            p3 = start[x+1][y+1];
            p4 = start[x+1][y];
            
            temp.b = p1.b*d1 + p2.b*d2 + p3.b*d3 + p4.b*d4;
            temp.g =p1.g*d1 + p2.g*d2 + p3.g*d3 + p4.g*d4;
            temp.r =p1.r*d1 + p2.r*d2 + p3.r*d3 + p4.r*d4;
            
            result[i][j] = temp;
        }
    }
    
}

