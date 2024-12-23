#pragma once
#include "wall.h"
#include <SDL.h>
#include <vector>

#define SPRITE_SIZE 64
#define WINDOW_RES_X 1280
#define WINDOW_RES_Y 720
#define GRID_MULTIPLIER 3

void create_level_grid_rects(std::vector<SDL_FRect*>&);
void show_grid(std::vector<SDL_FRect*>&, SDL_Renderer*);
void place_wall_pixels(std::vector<Wall>&, float, float, SDL_Renderer*);
void place_wall(std::vector<Wall>&, float, float, SDL_Renderer*);
void remove_wall_pixels(std::vector<Wall>& , float, float);