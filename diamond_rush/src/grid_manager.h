#pragma once
#include "wall.h"
#include "back_wall.h"
#include <SDL.h>
#include <vector>

#define SPRITE_SIZE 64
#define WINDOW_RES_X 1280
#define WINDOW_RES_Y 720
#define GRID_MULTIPLIER 3

void create_level_grid_rects(std::vector<SDL_FRect*>&);
void show_grid(std::vector<SDL_FRect*>&, SDL_Renderer*);
void remove_wall_pixels(std::vector<Wall>& , float, float);
void place_wall(int x, int y, SDL_Renderer* renderer, std::vector<Wall>& walls);
void place_back_wall(int x, int y, SDL_Renderer* renderer, std::vector<Back_Wall>& back_walls);