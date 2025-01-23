#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>

#define SPRITE_SIZE 64

class Player {
public:
    SDL_FRect rect;
    SDL_Texture* texture;
    float speed;


    Player(SDL_Renderer*,float,float);
    ~Player();
};
