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

int getMapVertices(const char* mapName ,float** buffer, int** indices){
    int faces = 0;
    int width, height;
    unsigned char* data = loadImageFromFile(mapName, &width, &height);
    if(!data){
        return 0;
    }
    
    (*buffer) = (float*)malloc(width * height * 3 * sizeof(float));
    (*indices) = (int*)malloc((width -1) * (height - 1) * 6 * sizeof(int));
    float stepSize = 1.0f / (float)width;
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            float* vert = (float*)(data[(x * 3) + (y * width * 3)]);
            (*buffer)[(x * 3) + (y * width * 3)    ] = x * stepSize; // x
            (*buffer)[(x * 3) + (y * width * 3) + 1] = 0;
            (*buffer)[(x * 3) + (y * width * 3) + 2] = y * stepSize;
            faces++;
        }
    }
    int dir = 1;
    for(int y = 0; y < height - 1; y++){
        for(int x = 0; x < width - 1; x++){
            int* face = &((*indices)[(y * width * 3 + x * 6)]);
            face[0] = x + (width * y);
            face[1] = x + (width * (y + 1));
        }
        dir *= -1;
    }
    return faces;
}

void freeMapVertices(float** buffer){
    free(*buffer);
}

#endif
