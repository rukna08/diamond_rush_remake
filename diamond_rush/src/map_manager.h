#pragma once
#include <vector>
#include "wall.h"
#include <fstream>
#include <sstream>
#include "grid_manager.h"
#include "wall.h"

// Temp definition
#define ANGKOR_WAT "data/map_config_angkor_wat.level"

void save_map(std::vector<Wall>&,int );
void reload_map(std::vector<Wall>&,SDL_Renderer*);
void destroy_map(std::vector<Wall>&);
