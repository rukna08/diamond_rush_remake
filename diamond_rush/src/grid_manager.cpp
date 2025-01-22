#include "grid_manager.h"



void show_grid(std::vector<SDL_FRect*>& level_grid, SDL_Renderer* rndr) {
	SDL_SetRenderDrawColor(rndr, 255, 255, 255, 30);
	for (int i = 0; i < level_grid.size(); i++) {
		SDL_RenderDrawRectF(rndr, level_grid[i]);
	}
}

void create_level_grid_rects(std::vector<SDL_FRect*>& level_grid) {
	for (float y = 0; y < WINDOW_RES_Y * GRID_MULTIPLIER; y += SPRITE_SIZE) {
		for (float x = 0; x < WINDOW_RES_X * GRID_MULTIPLIER; x += SPRITE_SIZE) {
			level_grid.push_back(new SDL_FRect{ x, y, SPRITE_SIZE, SPRITE_SIZE });
		}
	}
}


void remove_entity(float x, float y, std::vector<Entity*>& entities) {
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->rect.x == x && entities[i]->rect.y == y) {
			entities.erase(entities.begin() + i);
		}
	}
}

void place_entity(float x, float y, const std::string& type, std::vector<Entity*>& entities, SDL_Renderer* renderer) {	
	if (type == "wall") {
		for (int i = 0; i < entities.size(); i++) {
			if (entities[i]->rect.x == x && entities[i]->rect.y == y) {
				std::cout << "Entity already exists." << std::endl;
				return;
			}
		}
		entities.push_back(new Wall(x, y, renderer));
	}
	if (type == "back_wall") {
		for (int i = 0; i < entities.size(); i++) {
			if (entities[i]->rect.x == x && entities[i]->rect.y == y) {
				std::cout << "Entity already exists." << std::endl;
				return;
			}
		}
		entities.push_back(new Back_Wall(x, y, renderer));
	}

}
