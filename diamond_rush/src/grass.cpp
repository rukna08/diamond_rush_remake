#include "grass.h"

// This x and y is right now in screen coordinates
// instead of matrix coordinates. Maybe we should
// use matrix coordinates pretty soon.
Grass::Grass(int x, int y, SDL_Renderer* renderer) {
	rect.x = x;
	rect.y = y;
	rect.w = SPRITE_SIZE;
	rect.h = SPRITE_SIZE;

	texture = IMG_LoadTexture(renderer, "data/sprite_grass.png");
}