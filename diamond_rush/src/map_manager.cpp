#include "map_manager.h"


void save_map(std::vector<Wall>& stage_wall, std::vector<Back_Wall>& back_walls, int command) {
    std::ofstream angkor_level_file(ANGKOR_WAT);
    // [OPTIONAL] Delete everything on file before saving
    //angkor_level_file.open(ANGKOR_WAT, std::ofstream::out | std::ofstream::trunc);
    
    // we should create a vector of level items
    // level_items [] = [ walls*, player*, stones*, ... ]  <---- pseudo code btw
    // then iterate over the the level items and access their type and rect positions
    // to store the data in the file.


    for (int i = 0; i < back_walls.size(); i++) {
        angkor_level_file << back_walls[i].type << " " << back_walls[i].rect.x << " " << back_walls[i].rect.y << "\n";
    }
    for (int i = 0; i < stage_wall.size(); i++) {
        angkor_level_file << stage_wall[i].type << " " << stage_wall[i].rect.x << " " << stage_wall[i].rect.y << "\n";
    }
    angkor_level_file.close();
}


void reload_map(std::vector<Wall>& walls, std::vector<Back_Wall>& black_walls, SDL_Renderer* renderer) {
    std::ifstream angkor_level_file(ANGKOR_WAT);
    std::string line;
    while (std::getline(angkor_level_file, line)) {
        std::istringstream stream(line);
        std::string level_item_type;
        int x, y;
        if (stream >> level_item_type >> x >> y) {
            if (level_item_type == "wall") {
                place_wall(x, y, renderer, walls);
            }

            if (level_item_type == "back_wall") {
                place_back_wall(x, y, renderer, black_walls);
            }
        }
    }
    angkor_level_file.close();
}


void destroy_map(std::vector<Wall>& stage_wall) {
    if (stage_wall.size() == 0) return;
    else stage_wall.clear();
}
