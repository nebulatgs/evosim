#include "level.hpp"
// #include "globals.hpp"
// #include <SDL_opengles2.h>
#include <SDL.h>
#include <iostream>
// extern GLubyte* pixels;
// Level::Level(){}



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
    for(int i = 0; i < things.size(); i++){
        bool death = things[i] -> update(pixels, this);
        if (death){
            things.erase(things.begin() + i);
        }
        
    }
    // std::cout << (int)things.size() << '\n';
}

Level::~Level(){
    for(auto thing : things){
        delete thing;
        thing = nullptr;
    }
    things.empty();
}