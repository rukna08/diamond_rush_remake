#pragma once
#include <vector>
#include "wall.h"
#include <fstream>
#include <sstream>

// Temp definition
#define ANGKOR_WAT "data/map_config_angkor_wat.level"

void save_map(std::vector<Wall>& stage_wall, int command);
void reload_map();
void destroy_map(std::vector<Wall>& stage_wall);
