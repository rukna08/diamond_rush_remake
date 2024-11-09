#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

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

    Player(SDL_Renderer* renderer);
    void move(const std::string& direction);
    void move_constant_right();
    void move_constant_left();
    void move_constant_down();
    void move_constant_up();
};

#endif