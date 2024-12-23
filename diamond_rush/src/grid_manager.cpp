#include "grid_manager.h"



void show_grid(std::vector<SDL_FRect*>& level_grid, SDL_Renderer* rndr) {
	SDL_SetRenderDrawColor(rndr, 255, 255, 255, 30);
	for (int i = 0; i < level_grid.size(); i++) {
		SDL_RenderDrawRectF(rndr, level_grid[i]);
	}
}

void create_level_grid_rects(std::vector<SDL_FRect*>& level_grid) {

	for (int y = 0; y < WINDOW_RES_Y * GRID_MULTIPLIER; y += SPRITE_SIZE) {
		for (int x = 0; x < WINDOW_RES_X * GRID_MULTIPLIER; x += SPRITE_SIZE) {
			level_grid.push_back(new SDL_FRect{ (float)x, (float)y, SPRITE_SIZE, SPRITE_SIZE });

		}
	}
}

void place_wall_pixels(std::vector<Wall>& walls, float x, float y, SDL_Renderer* rndr) {
	walls.emplace_back(x, y, rndr);
}

void place_wall(std::vector<Wall>& walls, float unit_x, float unit_y, SDL_Renderer* rndr) {
	walls.emplace_back(SPRITE_SIZE * unit_x, SPRITE_SIZE * unit_y, rndr);
}

void remove_wall_pixels(std::vector<Wall>& walls, float x, float y) {
	auto it = std::remove_if(walls.begin(), walls.end(), [x, y](const Wall& wall) {
		return wall.rect.x == x && wall.rect.y == y;
		});


	if (it != walls.end()) {
		walls.erase(it, walls.end());
	}
}