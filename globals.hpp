#include <SDL_opengles2.h>
#include <SDL.h>
#include "v2d.h"
#include <emscripten.h>
#include <functional>
#include <iostream>
#include <filesystem>
#include <limits>
#include <fstream>
#include <iterator>
#include <vector>
#include <list>
#include <numeric>
#pragma once
// class Gene;

uint16_t screen_width, screen_height;
float zoomPhysics[3], scale;
v2d panPhysics[3], offset;
bool isMouseDown;
GLuint gridProgram, tilesProgram, tiles2Program;
SDL_Window *window;
std::list<float> frameAvg;

bool randDensity(int number){
    return (rand() % number) == number/2;
}

void getScreenSize(){
    screen_width = (uint16_t)EM_ASM_INT({
        var width = window.innerWidth
        || document.documentElement.clientWidth
        || document.body.clientWidth;
        return width;
    });
    screen_height = (uint16_t)EM_ASM_INT({
        var height = window.innerHeight
        || document.documentElement.clientHeight
        || document.body.clientHeight;
        return height;
    });
    // screen_width = screen_width > stg.init_width ? stg.init_width  : screen_width;
    // screen_height = screen_height > stg.init_height? stg.init_height : screen_height;
}

constexpr int64_t ipow(int64_t base, int exp, int64_t result = 1) {
  return exp < 1 ? result : ipow(base*base, exp/2, (exp % 2) ? result*base : result);
}
constexpr int power = 8;
// constexpr int tilesX = ipow(2, power) * 2;
// constexpr int tilesY = ipow(2, power);
// int tilesX;// = screen_width * 2;
// int tilesY;// = screen_height * 2;
GLubyte *pixels;//[tilesX*tilesY*3] = {255};
struct Settings{
    uint16_t screen_width, screen_height;
    uint16_t map_width, map_height;
    // const uint16_t tilesX, tilesY;
    // uint16_t init_width, init_height;
    float scale, drag;
};
Settings stg = {
    0, 0, // Screen Dimensions (init to zero, set later)
    0, 0, // Map Dimensions
    0, // Scale
    1.3f  // Drag Factor
};
struct Grid{
    Grid(Grid _src, Settings _stg):
        xDivs(_src.xDivs),
        yDivs(_src.yDivs),
        tWidth(_stg.map_width / _src.xDivs),
        tHeight(_stg.map_height / _src.yDivs){
    }
    Grid(uint32_t _xDivs, uint32_t _yDivs, Settings _stg):
        xDivs(_xDivs),
        yDivs(_yDivs),
        tWidth(_stg.map_width / xDivs),
        tHeight(_stg.map_height / yDivs){
    }
    uint32_t xDivs;
    uint32_t yDivs;
    uint32_t tWidth;
    uint32_t tHeight;
};