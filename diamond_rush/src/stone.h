#pragma once

#include "SDL.h"
#include <SDL_image.h>
#include <string>

#define SPRITE_SIZE 64

class Stone {
    public:
        SDL_FRect rect;
        SDL_Texture* texture;
        std::string object_name;
        
        Stone(float x, float y, std::string name, SDL_Renderer* renderer);
};

