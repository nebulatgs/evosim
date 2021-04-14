

#define GL_GLEXT_PROTOTYPES 1
#include "level.hpp"
#include "render.hpp"
Level *lvl;



void main_loop() { 
    handleEvents(zoomPhysics, panPhysics, 1.0f, 50.0f);

    // Calculate zoom and pan
    float maxScale = 1.0f;
    stg.scale = processPhysics(zoomPhysics, 1.1f, maxScale, 50)[2];
    offset = processPhysics(panPhysics, {1.1,1.1}, {1,1}, {stg.map_width * maxScale * 2 - (float)screen_width,stg.map_height * maxScale * 2 - (float)screen_height})[2];
    std::cout << offset.y << '\n'; 

    // Set canvas size and buffer the vertices for the quad
    setCanvas();
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Clear the screen
    glClearColor(0.086f, 0.086f, 0.086f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    std::cout << offset.x << '\n';
    lvl->draw(pixels);
    drawTiles2();

    if(stg.scale > 20)
        drawGrid();

    SDL_GL_SwapWindow(window);

    
}

bool randDensity(int number){
    return (rand() % number) == number/2;
}

int main()
{
    getScreenSize();
    // stg.init_width = screen_width;
    // stg.init_height = screen_height;
    SDL_CreateWindowAndRenderer(screen_width, screen_height, 0, &window, nullptr);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    initGL(window);

    Grid grid = Grid(stg.map_width, stg.map_height, stg);
    lvl = new Level(nullptr, grid.xDivs, grid.yDivs, grid.tWidth, grid.tHeight, 1 / stg.scale * 3, 0xFF434343);

    // Push food cells
    for(int i = 0; i < grid.xDivs * grid.yDivs; i++){
        int x = i % grid.xDivs * grid.tWidth;
        int y = i / grid.xDivs * grid.tHeight;
        if(x == 0 || y == 0 || i % grid.xDivs == grid.xDivs - 1 || i / grid.xDivs == grid.yDivs - 1){
            lvl->tiles.push_back(new Border(
                x,
                y,
                grid.tWidth,
                grid.tHeight
            ));
            continue;
        }
        lvl->tiles.push_back(new Resource(
            x,
            y,
            grid.tWidth,
            grid.tHeight,
            randDensity(1000)
        ));
    }
    std::cout << lvl->tiles[144]->x << '\n';

    emscripten_set_main_loop(main_loop, 0, true);

    return EXIT_SUCCESS;
}
