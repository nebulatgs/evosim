#include "level.hpp"
// #include "globals.hpp"
// #include <SDL_opengles2.h>
#include <SDL.h>
#include <iostream>
// extern GLubyte* pixels;
struct Settings{
    uint16_t screen_width, screen_height;
    uint16_t map_width, map_height;
    // const uint16_t tilesX, tilesY;
    // uint16_t init_width, init_height;
    float scale, drag;
    bool trails;
    float speed;
};
// enum class TileType {Wall, Resource, Creature};
extern Settings stg;
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
    int x = things.size();
    if(time(NULL) - oldTime > (6 / stg.speed)){
        oldTime = time(NULL);
        antibiotic = true;
        for (int i = 0; i < stg.map_height * stg.map_width * 3; i+=3){
            pixels[i] += 75;

        }
    }
    else{
        antibiotic = false;
    }
    // oldTime =  ? time(NULL) : oldTime;
    bool overPop;
    // overPop = things.size() > 3000;
    for(int i = 0; i < x; i++){
        bool death = things[i] -> update(pixels, this);
        if (death || (overPop && rand() % 1000 > 900 && things[i]->type == TileType::Creature)){
            // things[i] = new Border(things[i]->pos.x, things[i]->pos.y);
            delete things[i];
            things.erase(things.begin() + i);
            x--;
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