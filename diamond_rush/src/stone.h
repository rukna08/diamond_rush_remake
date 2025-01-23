#pragma once


#include "entity.h"


class Stone : public Entity {
    public:

        float fall_speed;

        Stone(float x, float y, SDL_Renderer* renderer);

        void fall(bool is_colliding) override;
};