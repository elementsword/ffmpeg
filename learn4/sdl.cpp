#include <iostream>
#include <fstream>
extern "C"
{
#include "SDL2/SDL.h"
}

int main()
{
    int width = 640;
    int height = 360;
    std::ifstream yuvFile("/root/learn/ffmpeg/audioFile/sintel_640_360.yuv", std::ios::binary);
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "error" << std::endl;
    }
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Rect *rect = new SDL_Rect;
    // 中间显示 800宽 600高 立刻显示
    window = SDL_CreateWindow("SDL2",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height,
                              SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV,
                                SDL_TEXTUREACCESS_STREAMING,
                                width, height);

    int frameSize = width * height * 3 / 2;
    // 存放每帧
    uint8_t *buffer = new uint8_t[frameSize];

    while (yuvFile.read((char *)buffer, frameSize))
    {
        // 获取 Y U V 三个分量的指针 Y = x*y U,V=x*y/4
        uint8_t *yPlane = buffer;
        uint8_t *uPlane = buffer + width * height;
        uint8_t *vPlane = buffer + width * height * 5 / 4;
        // SDL_UpdateYUVTexture(texture,);
        SDL_UpdateTexture(texture, nullptr, buffer, width);
        rect->x = 0;
        rect->y = 0;
        rect->w = width;
        rect->h = height;
        SDL_RenderClear(renderer);
        //
        SDL_RenderCopy(renderer, texture, nullptr, rect);
        SDL_RenderPresent(renderer);
        // Delay 40ms 每秒显示40帧
        SDL_Delay(80);
    }
    delete rect;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}