#pragma once
#include "entity.h"

class Grass : public Entity {
public:
	bool trampled;
	Grass(float x, float y, SDL_Renderer* renderer);
};