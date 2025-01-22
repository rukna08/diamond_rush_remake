#pragma once
#include <vector>
#include "wall.h"
#include <fstream>
#include <sstream>
#include "grid_manager.h"
#include "wall.h"
#include "back_wall.h"

// Temp definition
#define ANGKOR_WAT "data/map_config_angkor_wat.level"

void save_map(std::vector<Entity*>& entities, int command);
void reload_map(std::vector<Entity*> entities, SDL_Renderer* renderer);
void destroy_map(std::vector<Wall>&, std::vector<Back_Wall>&);
