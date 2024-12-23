#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include "wall.h"

#define SPRITE_SIZE 64

class Player {
public:
    SDL_FRect rect;
    SDL_Texture* texture;
    

    Player(SDL_Renderer*,float,float);
    ~Player();
   // void check_collision(std::vector<Wall>&);
};
