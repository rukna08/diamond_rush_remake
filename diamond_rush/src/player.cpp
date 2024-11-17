#include "player.h"

Player::Player(SDL_Renderer* renderer, int unit_x, int unit_y) {
    rect = { unit_x * SPRITE_SIZE, unit_y * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE };
    destination = 0;
    can_move_right = false;
    can_move_left = false;
    can_move_up = false;
    can_move_down = false;
    texture = IMG_LoadTexture(renderer, "data/animation/player_idle/0.png");
    speed = 1;
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
        rect.x += speed;
    }
}

void Player::move_constant_left() {
    if (rect.x == destination) {
        can_move_left = false;
    }
    if (can_move_left && rect.x > destination) {
        rect.x -= speed;
    }
}

void Player::move_constant_down() {
    if (rect.y == destination) {
        can_move_down = false;
    }
    if (can_move_down && rect.y < destination) {
        rect.y += speed;
    }
}

void Player::move_constant_up() {
    if (rect.y == destination) {
        can_move_up = false;
    }
    if (can_move_up && rect.y > destination) {
        rect.y -= speed;
    }
}

void Player::check_collision( std::vector<Wall>& walls) {
    
    SDL_Point right_tile_point = { rect.x + (SPRITE_SIZE), rect.y };
    SDL_Point left_tile_point  = { rect.x - 1, rect.y };
    SDL_Point up_tile_point    = { rect.x, rect.y - 1 };
    SDL_Point down_tile_point  = { rect.x, rect.y + SPRITE_SIZE };

    for (int i = 0; i < walls.size(); i++) {
        if (SDL_PointInRect(&right_tile_point, &walls[i].rect)) {
            can_move_right = false;
        } else if (SDL_PointInRect(&left_tile_point, &walls[i].rect)) {
            can_move_left  = false;
        } else if (SDL_PointInRect(&up_tile_point, &walls[i].rect)) {
            can_move_up    = false;
        } else if (SDL_PointInRect(&down_tile_point, &walls[i].rect)) {
            can_move_down  = false;
        }
    }
}