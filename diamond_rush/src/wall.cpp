#include "wall.h"


Wall::Wall(int x,int y,SDL_Renderer* renderer) {
	rect = {x,y,SPRITE_SIZE,SPRITE_SIZE};
	texture = IMG_LoadTexture(renderer, "data/sprite_wall.png");
	type = "wall";
}