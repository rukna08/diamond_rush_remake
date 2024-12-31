#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>

// we are including this in every file, wtf?
// let's try to figure out how to make SPRITE_SIZE be
// global accross all the src files.
#define SPRITE_SIZE 64

class Back_Wall {
public:
	SDL_Rect rect;
	SDL_Texture* texture;
	std::string type; // should we use string for our type or Enum::LevelItem -> int ??
	
	Back_Wall(int x, int y, SDL_Renderer* renderer);
};