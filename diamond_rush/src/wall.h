#pragma once
#define SPRITE_SIZE 64

#include <SDL.h>
#include <SDL_image.h>
#include <string>


class Wall {
public:
	SDL_Rect rect;
	SDL_Texture* texture;
	std::string type;

	Wall(int x,int y, SDL_Renderer* renderer);
};