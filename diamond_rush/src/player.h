#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include "wall.h"

#define SPRITE_SIZE 64

class Player {
public:
    SDL_Rect rect;
    SDL_Texture* texture;
    int destination;
    bool can_move_right;
    bool can_move_left;
    bool can_move_up;
    bool can_move_down;
    int speed;

    Player(SDL_Renderer*,int,int);
    void move(const std::string& );
    void move_constant_right();
    void move_constant_left();
    void move_constant_down();
    void move_constant_up();
    void check_collision( std::vector<Wall>&);
};
