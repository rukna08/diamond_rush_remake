#include "stone.h"

Stone::Stone(float x, float y, std::string name, SDL_Renderer* renderer) {
	rect = { x, y, SPRITE_SIZE, SPRITE_SIZE };
	texture = IMG_LoadTexture(renderer, "data/animation/stone_move/0.png");
	object_name = name;
}