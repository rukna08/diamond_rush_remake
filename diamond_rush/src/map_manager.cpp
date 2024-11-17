#include "map_manager.h"

// [ENGINE MODE] Save the walls upon a KEYPRESS 'r' event.
void save_map(std::vector<Wall>& stage_wall, int command) {
    std::ofstream angkor_level_file(ANGKOR_WAT);
    if (stage_wall.size() == 0) return;
    for (int i = 0; i < stage_wall.size(); i++) {
        angkor_level_file << stage_wall[i].rect.x << " " << stage_wall[i].rect.y << "\n";
    }
    angkor_level_file.close();
}

// [GAME MODE] Reload the walls upon a KEYPRESS 'y' event.
void reload_map(std::vector<Wall>& stage_wall, SDL_Renderer* rndr) {
    std::ifstream angkor_level_file(ANGKOR_WAT);
    std::string line;
    while (std::getline(angkor_level_file, line)) {
        std::istringstream stream(line);
        int x, y;
        if (stream >> x >> y) {
            place_wall_pixels(stage_wall,x, y,rndr);
        }
    }
    angkor_level_file.close();
}

// [GAME MODE] Destroy the walls upon a KEYPRESS 'l' event.
void destroy_map(std::vector<Wall>& stage_wall) {
    if (stage_wall.size() == 0) return;
    else stage_wall.clear();
}
