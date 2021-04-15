

#define GL_GLEXT_PROTOTYPES 1
#include "level.hpp"
#include "render.hpp"
Level *lvl;

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
void main_loop() { 
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
    handleEvents(zoomPhysics, panPhysics, 1.0f, 50.0f);

    // Calculate zoom and pan
    float maxScale = 1.0f;
    stg.scale = processPhysics(zoomPhysics, stg.drag, maxScale, 50)[2];
    v2d clipEnd = {stg.map_width * maxScale * stg.scale*2 - (float)screen_width,stg.map_height * maxScale * stg.scale*2 - (float)screen_height};
    v2d clipStart = {-(stg.map_width * maxScale * stg.scale*2 - (float)screen_width)/2,-(stg.map_height * maxScale * stg.scale*2 - (float)screen_height)/2};

    offset = processPhysics(panPhysics, {stg.drag, stg.drag}, clipStart, clipEnd)[2];
    offset += v2d(stg.map_width/2, stg.map_height/2) * v2d(stg.scale -1, stg.scale - 1) * 2;

    offset.x = offset.x > clipEnd.x ? clipEnd.x : offset.x;
    offset.y = offset.y > clipEnd.y ? clipEnd.y : offset.y;

    // if(offset.x < clipStart.x){
    //     offset.x = clipStart.x;
    // }
    // if(offset.y < clipStart.y){
    //     offset.y = clipStart.y;
    // }

    // offset /= 2;
    std::cout << stg.scale << '\n'; 

    // Set canvas size and buffer the vertices for the quad
    setCanvas();
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Clear the screen
    glClearColor(0.086f, 0.086f, 0.086f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // std::cout << offset.x << '\n';
    memset(pixels, 0, stg.map_height * stg.map_width * 3);
    lvl->update(pixels);
    drawTiles2();

    if(stg.scale > 10)
        drawGrid();

    SDL_GL_SwapWindow(window);
}



int main()
{
    srand(time(nullptr));
    getScreenSize();
    stg.map_height = screen_height/2;
    stg.map_width = screen_width/2;
    pixels = new GLubyte[stg.map_width * 2*stg.map_height * 2*3];
    // stg.init_width = screen_width;
    // stg.init_height = screen_height;
    SDL_CreateWindowAndRenderer(screen_width, screen_height, 0, &window, nullptr);

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

    // Push food cells
    for(int i = 0; i < grid.xDivs * grid.yDivs; i++){
        int x = i % grid.xDivs * grid.tWidth;
        int y = i / grid.xDivs * grid.tHeight;
        if(x == 0 || y == 0 || i % grid.xDivs == grid.xDivs - 1 || i / grid.xDivs == grid.yDivs - 1){
            lvl->things.push_back(new Border(
                x,
                y
            ));
            continue;
        }
        if(randDensity(2000)){
        lvl->things.push_back(new Resource(
            x,
            y,
            1
        ));}
        if(randDensity(100000)){
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
