#include "level.hpp"
// #include "globals.hpp"
// #include <SDL_opengles2.h>
#include <SDL.h>
// extern GLubyte* pixels;
Level::Level(SDL_Renderer *_renderer, uint16_t _tilesX, uint16_t _tilesY, uint16_t _tileW, uint16_t _tileH, uint8_t strokeWidth, uint32_t color) :
    renderer(_renderer), tilesX(_tilesX), tilesY(_tilesY), tileW(_tileW), tileH(_tileH), strokeWidth(strokeWidth), color(color){
        // things.reserve(tilesX * tilesY);
    }



// void Level::draw(uint8_t *pixels){
//     // SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
//     // SDL_RenderClear(renderer);
//     for(auto x : tiles){
//         x -> draw(pixels);
//     }
//     // int i, j, c;
//     // int index = 0;
//     // for (i = 0; i < tilesX; i++) {
//     //     for (j = 0; j < tilesY; j++) {
//     //         pixels[index++] = i < 5 && j < 5 ? 255 : 0;
//     //         pixels[index++] = i < 5 && j < 5 ? 255 : 0;
//     //         pixels[index++] = i < 5 && j < 5 ? 255 : 0;        
//     //     }
//     // }
//     // SDL_RenderPresent(renderer);
// }
void Level::update(uint8_t *pixels){
    for(auto x : things){
        x -> update(pixels);
    }
}
