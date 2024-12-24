#include "player.h"

Player::Player(SDL_Renderer* renderer, float unit_x, float unit_y) {
    rect = { unit_x * SPRITE_SIZE, unit_y * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE };
    texture = IMG_LoadTexture(renderer, "data/animation/player_idle/0.png");
}

Player::~Player() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}