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


void remove_wall_pixels(std::vector<Wall>& walls, float x, float y) {
	auto it = std::remove_if(walls.begin(), walls.end(), [x, y](const Wall& wall) {
		return wall.rect.x == x && wall.rect.y == y;
		});


	if (it != walls.end()) {
		walls.erase(it, walls.end());
	}
}

void place_back_wall(int x, int y, SDL_Renderer* renderer, std::vector<Back_Wall>& back_walls) {
	back_walls.push_back(Back_Wall(x, y, renderer));
}

void place_wall(int x, int y, SDL_Renderer* renderer, std::vector<Wall>& walls) {
	walls.push_back(Wall(x, y, renderer));
}

// we should make a generic function which takes in a level_item_type
// and draws it. same for placing them.
void draw_back_walls(SDL_Renderer* renderer, std::vector<Back_Wall>& back_walls) {
	for (int i = 0; i < back_walls.size(); i++) {
		SDL_RenderCopy(renderer, back_walls[i].texture, 0, &back_walls[i].rect);
	}
}