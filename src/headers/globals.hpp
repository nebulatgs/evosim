#pragma once

#include <SDL_opengles2.h>
#include <SDL.h>
#include "v2d.h"
#include <emscripten.h>
#include <list>

#include "settings.hpp"

// extern uint16_t screen_width;
// extern uint16_t screen_height;
extern float zoomPhysics[3];
extern float scale;
extern v2d panPhysics[3];
extern v2d offset;
extern bool isMouseDown;
extern bool firefox;
extern GLuint gridProgram;
extern GLuint tilesProgram;
extern GLuint tiles2Program;
extern GLuint fbo;
extern GLuint textureID;
extern GLubyte *pixels;
extern SDL_Window *window;
extern std::list<float> frameAvg;
extern bool reverse_a;
extern Settings stg;

bool randDensity(int number);
void getScreenSize();
void getBrowser();
