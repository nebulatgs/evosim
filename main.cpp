

#define GL_GLEXT_PROTOTYPES 1
#include "level.hpp"
#include "render.hpp"
#include <unistd.h>
Level *lvl;
v2d cursor = v2d(0,0);
bool frame;
float _speed;
// float CalculateAvg(const std::list<float> &list)
// {
//     float avg = 0;
//     std::list<float>::const_iterator it;
//     for(it = list.begin(); it != list.end(); it++) avg += *it;
//     avg /= list.size();
// }
float listAvg(const std::list<float> &list){
    std::accumulate(list.begin(), list.end(), 0.0f) / list.size();
}
uint64_t startTime, endTime;

#define  Pr  .299
#define  Pg  .587
#define  Pb  .114
void changeSaturation(double *R, double *G, double *B, double change) {

  double  P=sqrt(
  (*R)*(*R)*Pr+
  (*G)*(*G)*Pg+
  (*B)*(*B)*Pb ) ;

  *R=P+((*R)-P)*change;
  *G=P+((*G)-P)*change;
  *B=P+((*B)-P)*change; }
extern "C" void EMSCRIPTEN_KEEPALIVE setFade(bool fade) { stg.trails = fade; }
extern "C" void EMSCRIPTEN_KEEPALIVE changePause(bool pause, float speed) { stg.speed = pause ? 0 : speed; _speed = speed;}
extern "C" void EMSCRIPTEN_KEEPALIVE setSpeed(float speed) { stg.speed = speed; }
extern "C" void EMSCRIPTEN_KEEPALIVE reverseA(bool a) { reverse_a = a; }
// std::cout << stg.speed << '\n';
extern "C" void EMSCRIPTEN_KEEPALIVE nextFrame(float speed) { _speed = speed; frame = 1;}

int main();

extern "C" void EMSCRIPTEN_KEEPALIVE restart(){
    emscripten_cancel_main_loop();
    memset(pixels, 0, stg.map_height * stg.map_width * 3);
    delete[] pixels;
    pixels = nullptr;
    delete lvl;
    main();
}

void main_loop() { 
    if(stg.speed == 0 && frame){
        stg.speed = _speed;
    }
    endTime = SDL_GetPerformanceCounter();
    float elapsed = (endTime - startTime) / (float)SDL_GetPerformanceFrequency();
    frameAvg.push_back(1.0f/elapsed);
    if(frameAvg.size() > 20)
        frameAvg.pop_front();
    float sum = 0;
    for(auto x : frameAvg){
        sum += x;
    }
    // std::cout << "Current FPS: " << std::to_string((int)round(sum/20.0f)) << '\n';
    startTime = SDL_GetPerformanceCounter();
    handleEvents(&cursor, zoomPhysics, panPhysics, 1.0f, 50.0f);

    // Calculate zoom and pan
    float maxScale = 1.0f;
    stg.scale = processPhysics(zoomPhysics, stg.drag, maxScale, 50)[2];
    // v2d clipEnd = {stg.map_width * stg.scale * 2 - (float)stg.map_width, stg.map_height * stg.scale * 2 - (float)stg.map_height};
    // v2d clipStart = {-(stg.map_width * stg.scale * 2 - (float)stg.map_width)/2, -(stg.map_height * stg.scale * 2 - (float)stg.map_height)/2};
    v2d clipEnd = {stg.map_width * maxScale * stg.scale*2 - (float)screen_width,stg.map_height * maxScale * stg.scale*2 - (float)screen_height};
    v2d clipStart = {-(stg.map_width * maxScale * stg.scale*2 - (float)screen_width)/2,-(stg.map_height * maxScale * stg.scale*2 - (float)screen_height)/2};
    // std::cout << clipEnd.x << '\n';
    offset = processPhysics(panPhysics, {stg.drag, stg.drag}, clipStart, clipEnd)[2];
    v2d temp = cursor;
    temp.x /= stg.map_width/2;
    temp.y = stg.map_height - temp.y;
    temp.y /= stg.map_height/2;
    temp.y +=2;
    // std::cout << temp.y << '\n';
    offset += v2d(stg.map_width/(2 * maxScale), stg.map_height/(2 * maxScale)) * v2d(stg.scale -maxScale, stg.scale - maxScale) * temp;

    offset.x = offset.x > clipEnd.x ? clipEnd.x : offset.x;
    offset.y = offset.y > clipEnd.y ? clipEnd.y : offset.y;

    offset.x = offset.x < clipStart.x ? clipStart.x : offset.x;
    offset.y = offset.y < clipStart.y ? clipStart.y : offset.y;

    // offset /= 2;
    // std::cout << stg.scale << '\n'; 

    // Set canvas size and buffer the vertices for the quad
    setCanvas();
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Clear the screen
    glClearColor(0.086f, 0.086f, 0.086f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // std::cout << offset.x << '\n';
    if(!stg.trails)
        memset(pixels, 0, stg.map_height * stg.map_width * 3);
    else{
        for (int i = 0; i < stg.map_height * stg.map_width * 3; i+=3){
            pixels[i] = pixels[i] <= 0x27 ? 0 : pixels[i] - 1;
            pixels[i+1] = pixels[i+1] <= 0x27 ? 0 : pixels[i+1] - 1;
            pixels[i+2] = pixels[i+2] <= 0x27 ? 0 : pixels[i+2] * 2 - 1;
        }
    }
    
    lvl->update(pixels);
    glGenFramebuffers(1, &fbo);
    drawTiles2();

    if(stg.scale > 10)
        drawGrid();

    SDL_GL_SwapWindow(window);
    if(frame){
        frame = 0;
        _speed = 0;
        stg.speed = 0;
    }
    // emscripten_sleep(stg.sleep);
    // usleep(stg.sleep * 1000);
}



int main()
{
    getBrowser();
    srand(time(nullptr));
    getScreenSize();
    stg.map_height = screen_height/2;
    stg.map_width = screen_width/2;
    pixels = new GLubyte[stg.map_width * 2*stg.map_height * 2*3];
    // stg.init_width = screen_width;
    // stg.init_height = screen_height;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(screen_width, screen_height, 0, &window, &renderer);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetSwapInterval(1);
    
    initGL(window);

    ecosystem.push_back({
        {{{1}, 1}},
        {{{2}, 2}},
        {{{2}, 3}}
    });
    Grid grid = Grid(stg.map_width, stg.map_height, stg);
    lvl = new Level();
    lvl->oldTime = time(NULL) - ((6 / stg.speed) - 1);

    // Push food cells
    for(int i = 0; i < grid.xDivs * grid.yDivs; i++){
        int x = i % grid.xDivs * grid.tWidth;
        x += 1;
        int y = i / grid.xDivs * grid.tHeight;
        y += 1;
        if(x == 0 || y == 0 || i % grid.xDivs == grid.xDivs - 1 || i / grid.xDivs == grid.yDivs - 1){
            lvl->things.push_back(new Border(
                x,
                y
            ));
            continue;
        }
        if(randDensity(1000)){
        lvl->things.push_back(new Resource(
            x,
            y,
            1
        ));}
        if(randDensity(10000)){
        lvl->things.push_back(new Creature(
            x,
            y,
            0
        ));}
    }
    // std::cout << lvl->things[14]->x << '\n';

    emscripten_set_main_loop(main_loop, -1, true);

    return EXIT_SUCCESS;
}
