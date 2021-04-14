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


constexpr int64_t ipow(int64_t base, int exp, int64_t result = 1) {
  return exp < 1 ? result : ipow(base*base, exp/2, (exp % 2) ? result*base : result);
}
constexpr int power = 10;
constexpr int tilesX = ipow(2, power);
constexpr int tilesY = ipow(2, power);
GLubyte pixels[tilesX*tilesY*3] = {255};
struct Settings{
    uint16_t screen_width, screen_height;
    const uint16_t map_width, map_height;
    // const uint16_t tilesX, tilesY;
    // uint16_t init_width, init_height;
    float scale, drag;
};
Settings stg = {
    0, 0, // Screen Dimensions (init to zero, set later)
    tilesX, tilesY, // Map Dimensions
    0, // Scale
    1.5f  // Drag Factor
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