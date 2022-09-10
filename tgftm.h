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

void generateMap(float** vert, float step = 0.1f, int width = 0, int height = 0){
    Square* squares = (Square*)malloc(width * height * sizeof(Square));
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            //  p1 = = p3
            //  ||     ||  
            //  ||     ||
            //  p2 = = p4
            float x1_y = 0.0f;
            float x2_y = 0.0f;
            Vector3 p1{x * step, x1_y, y * step};
            Vector3 p2{x * step,x1_y, (y + 1) * step};
            Vector3 p3{(x + 1) * step, x2_y, y * step};
            Vector3 p4{(x + 1) * step, x2_y, (y + 1) * step};

            squares[x + (y * width)] = Square{Triangle{p1, p2, p3}, Triangle{p2, p3, p4}};
        }
    }
    (*vert) = (float*)(squares);
}

#endif
