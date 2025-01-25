#pragma once

#include "wall.h"
#include "back_wall.h"
#include "stone.h"

#include <SDL.h>
#include <vector>
#include <iostream>

#define SPRITE_SIZE 64
#define WINDOW_RES_X 900
#define WINDOW_RES_Y 600
#define GRID_MULTIPLIER 3

void create_level_grid_rects(std::vector<SDL_FRect*>&);
void show_grid(std::vector<SDL_FRect*>&, SDL_Renderer*);
void place_entity(float x, float y, const std::string& type, std::vector<Entity*>& entities, SDL_Renderer* renderer);
void remove_entity(float x, float y, std::vector<Entity*>& entities);