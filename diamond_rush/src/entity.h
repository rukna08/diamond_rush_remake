#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>

#define SPRITE_SIZE 64



class Entity {
public:
	SDL_FRect rect;
	SDL_Texture* texture;
	std::string type;
};