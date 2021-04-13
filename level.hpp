#include "tile.hpp"
#pragma once
class SDL_Texture;



class Level{
    public:
        Level(SDL_Renderer *renderer, uint16_t _tilesX, uint16_t _tilesY, uint16_t _tileW, uint16_t _tileH, uint8_t strokeWidth, uint32_t color);
        uint16_t tilesX, tilesY, tileW, tileH;
        uint8_t strokeWidth;
        uint32_t color;
        SDL_Texture* gridTile;
        SDL_Renderer* renderer;
        std::vector<Tile*> tiles;
        void draw();
        void update();
};