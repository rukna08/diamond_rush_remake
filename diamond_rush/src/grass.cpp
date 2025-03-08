#include "grass.h"

Grass::Grass(float x, float y, SDL_Renderer* renderer) {
	rect = {
		x, y,
		SPRITE_SIZE, SPRITE_SIZE
	};
	texture = IMG_LoadTexture(renderer, "data/sprite_grass.png");
	trampled = false;
	type = "grass";
}