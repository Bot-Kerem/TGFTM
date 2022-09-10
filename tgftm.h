// github: Bot-Kerem
// single header topographic 3d terrain generator

#ifndef TGFTM_H
#define TGFTM_H

#if 1
    #define STB_IMAGE_IMPLEMENTATION
    #include "stb_image.h"
#endif
//TGFTM_USE_STB

unsigned char* loadImageFromFile(const char* path, int* width, int* height){
    stbi_set_flip_vertically_on_load(1);
    
    int nrChannels;
    unsigned char *data = stbi_load(path, width, height, &nrChannels, 3);
    return data;
}

void freeImage(void* data){
    stbi_image_free(data);
}

struct Vector3{
    float x;
    float y;
    float z;
};

struct Triangle{
    Vector3 p1;
    Vector3 p2;
    Vector3 p3;
};

struct Square{
    Triangle t1;
    Triangle t2;
};

void generateMap(const char* path, float** vert, int* w, int* h, float step = 0.1f){
    int img_width, img_height;
    
    unsigned char* image_data = loadImageFromFile(path, &img_width, &img_height);
    if(!image_data){
        return;
    }

     int width = img_width; int height = img_height;
    Square* squares = (Square*)malloc(width * height * sizeof(Square));
    
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            //  p1 = = p3
            //  ||     ||  
            //  ||     ||
            //  p2 = = p4
            float x0_y0_h = (((image_data[(x * 3) + (y * img_width * 3)] / 255.0f) * 2) - ((image_data[((x * 3)+ 2) + (y * img_width * 3)]) / 255.0f) - ((image_data[((x * 3) + 1) + (y * img_width * 3)]) / 255.0f));
            float x1_y0_h = (((image_data[((x + 1) * 3) + (y * img_width * 3)] / 255.0f) * 2) - ((image_data[(((x + 1) * 3)+ 2) + (y * img_width * 3)]) / 255.0f) - ((image_data[(((x + 1) * 3) + 1) + (y * img_width * 3)]) / 255.0f));
            float x0_y1_h = (((image_data[(x * 3) + ((y + 1) * img_width * 3)] / 255.0f) * 2) - ((image_data[((x * 3)+ 2) + ((y + 1) * img_width * 3)]) / 255.0f) - ((image_data[((x * 3) + 1) + ((y + 1) * img_width * 3)]) / 255.0f));
            float x1_y1_h = (((image_data[((x + 1) * 3) + ((y + 1) * img_width * 3)] / 255.0f) * 2) - ((image_data[(((x + 1) * 3)+ 2) + ((y + 1) * img_width * 3)]) / 255.0f) - ((image_data[(((x + 1) * 3) + 1) + ((y + 1) * img_width * 3)]) / 255.0f));
            Vector3 p1{x * step, x0_y0_h, y * step};
            Vector3 p2{x * step,x0_y1_h, (y + 1) * step};
            Vector3 p3{(x + 1) * step, x1_y0_h, y * step};
            Vector3 p4{(x + 1) * step, x1_y1_h, (y + 1) * step};

            squares[x + (y * width)] = Square{Triangle{p1, p2, p3}, Triangle{p2, p3, p4}};
        }
    }
    (*vert) = (float*)(squares);
    (*w) = width;
    (*h) = height;
    freeImage(image_data);
}

#endif
