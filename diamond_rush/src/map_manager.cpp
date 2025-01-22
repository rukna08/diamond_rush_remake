#include "map_manager.h"


void save_map(std::vector<Entity*>& entities, int command) { // What the fuck is this command doing here?
    std::ofstream angkor_level_file(ANGKOR_WAT);
    
    for (int i = 0; i < entities.size(); i++) {
        angkor_level_file << entities[i]->type << " " << entities[i]->rect.x << " " << entities[i]->rect.y << "\n";
    }
    angkor_level_file.close();

    std::cout << "Map saved." << std::endl;
}


void reload_map(std::vector<Entity*>& entities, SDL_Renderer* renderer) {
    std::ifstream angkor_level_file(ANGKOR_WAT);
    std::string line;
    while (std::getline(angkor_level_file, line)) {
        std::istringstream stream(line);
        std::string level_item_type;
        int x, y;
        if (stream >> level_item_type >> x >> y) {
            place_entity(x, y, level_item_type, entities, renderer);
        }
    }
    angkor_level_file.close();
}


void destroy_map(std::vector<Entity*>& entities) {
    if (entities.size() == 0) return;
    else entities.clear();
}
