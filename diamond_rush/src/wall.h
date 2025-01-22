#pragma once


#include "entity.h"


class Wall : public Entity {
public:
	Wall(float x, float y, SDL_Renderer* renderer);
};