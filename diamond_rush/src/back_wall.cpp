#include "back_wall.h"

Back_Wall::Back_Wall(float x, float y, SDL_Renderer* renderer) {
	
	rect = {
		x, y,
		SPRITE_SIZE, SPRITE_SIZE
	};

	texture = IMG_LoadTexture(renderer, "data/sprite_back_wall.png");
	
	type = "back_wall";
}