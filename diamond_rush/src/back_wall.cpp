#include "back_wall.h"

Back_Wall::Back_Wall(int x, int y, SDL_Renderer* renderer) {
	rect.x = x;
	rect.y = y;
	rect.w = SPRITE_SIZE;
	rect.h = SPRITE_SIZE;
	texture = IMG_LoadTexture(renderer, "data/sprite_back_wall.png");
	type = "back_wall";
}