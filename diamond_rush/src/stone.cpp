#include "stone.h"

Stone::Stone(float x, float y, SDL_Renderer* renderer) {
	

	// Remove this from all entity children and just add this into the Entity class itself.
	rect = {
		x, y,
		SPRITE_SIZE, SPRITE_SIZE
	};

	texture = IMG_LoadTexture(renderer, "data/sprite_stone.png");

	type = "stone";

	fall_speed = 0.4;
}

void Stone::fall(bool is_colliding) {
	
	if (!is_colliding) {
		rect.y += fall_speed;
	}


}