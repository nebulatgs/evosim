#include "level.hpp"
#include <SDL.h>

Level::Level(SDL_Renderer *_renderer, uint16_t _tilesX, uint16_t _tilesY, uint16_t _tileW, uint16_t _tileH, uint8_t strokeWidth, uint32_t color) :
    renderer(_renderer), tilesX(_tilesX), tilesY(_tilesY), tileW(_tileW), tileH(_tileH), strokeWidth(strokeWidth), color(color){
        tiles.reserve(tilesX * tilesY);
    }



void Level::draw(){
    // SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    // SDL_RenderClear(renderer);
    // for(auto x : tiles){
    //     x -> draw(renderer);
    // }

    // SDL_RenderPresent(renderer);
}
void Level::update(){
    for(auto x : tiles){
        x -> update(&tiles);
    }
}
