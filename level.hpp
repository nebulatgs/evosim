#include "tile.hpp"
#pragma once
class SDL_Texture;



class Level{
    public:
        // Level();
        ~Level();
        std::vector<Thing*> things;
        void update(uint8_t *pixels);
        // void update();
};