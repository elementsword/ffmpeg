#include <iostream>

extern "C"
{
#include "SDL2/SDL.h"
}
int main() { return SDL_Init(SDL_INIT_VIDEO) == 0 ? (std::cout << "SDL OK\n", SDL_Quit(), 0) : (std::cerr << "SDL Error: " << SDL_GetError() << "\n", 1); }