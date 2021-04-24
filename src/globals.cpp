#include "headers/globals.hpp"

uint16_t screen_width;
uint16_t screen_height;
float zoomPhysics[3];
float scale;
v2d panPhysics[3];
v2d offset;
bool isMouseDown;
bool firefox;
GLuint gridProgram;
GLuint tilesProgram;
GLuint tiles2Program;
GLuint fbo;
GLuint textureID;
GLubyte *pixels;
SDL_Window *window;
std::list<float> frameAvg;
bool reverse_a = false;
Settings stg = {
    0, 0, // Screen Dimensions (init to zero, set later)
    0, 0, // Map Dimensions
    0, // Scale
    1.3f, // Drag Factor
    false,
    1
};

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

void getBrowser(){
    firefox = (uint16_t)EM_ASM_INT({
        // var browser = bowser.detect
        // var bowser;
        // var browser = bowser.getParser(window.navigator.userAgent);
        // var name = browser.getBrowser().name;
        // return name=="Firefox";
        return 0;
    });
}

