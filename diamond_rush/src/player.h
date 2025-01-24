#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>

#include "entity.h"

#define SPRITE_SIZE 64

class Player : public Entity {
public:
    SDL_FRect rect;
    SDL_Texture* texture;
    float speed;


    Player(SDL_Renderer*,float,float);
    ~Player();
};
