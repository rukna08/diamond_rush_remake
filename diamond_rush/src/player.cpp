#include "player.h"

Player::Player(SDL_Renderer* renderer) {
    rect = { 0, 0, SPRITE_SIZE, SPRITE_SIZE };
    destination = 0;
    can_move_right = false;
    can_move_left = false;
    can_move_up = false;
    can_move_down = false;
    texture = IMG_LoadTexture(renderer, "data/sprite_player.png");
}

void Player::move(const std::string& direction) {
    if (!can_move_up && !can_move_down && !can_move_left && !can_move_right) {
        if (direction == "right") {
            destination = rect.x + SPRITE_SIZE;
            can_move_right = true;
        }
        else if (direction == "left") {
            destination = rect.x - SPRITE_SIZE;
            can_move_left = true;
        }
        else if (direction == "up") {
            destination = rect.y - SPRITE_SIZE;
            can_move_up = true;
        }
        else if (direction == "down") {
            destination = rect.y + SPRITE_SIZE;
            can_move_down = true;
        }
    }
}

void Player::move_constant_right() {
    if (rect.x == destination) {
        can_move_right = false;
    }
    if (can_move_right && rect.x < destination) {
        rect.x++;
    }
}

void Player::move_constant_left() {
    if (rect.x == destination) {
        can_move_left = false;
    }
    if (can_move_left && rect.x > destination) {
        rect.x--;
    }
}

void Player::move_constant_down() {
    if (rect.y == destination) {
        can_move_down = false;
    }
    if (can_move_down && rect.y < destination) {
        rect.y++;
    }
}

void Player::move_constant_up() {
    if (rect.y == destination) {
        can_move_up = false;
    }
    if (can_move_up && rect.y > destination) {
        rect.y--;
    }
}