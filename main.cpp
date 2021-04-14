

#define GL_GLEXT_PROTOTYPES 1
#include "level.hpp"
#include "render.hpp"
Level *lvl;

// extern auto tiles2Program();

void main_loop() { 
    handleEvents(zoomPhysics, panPhysics, 1.0f, 50.0f);

    // Calculate zoom and pan
    stg.scale = processPhysics <float> (zoomPhysics, 1.1f, 1.0f, 50)[2];
    offset = processPhysics <v2d> (panPhysics, {1.1,1.1}, {0,0}, {-50000,-50000})[2];

    // Set canvas size and buffer the vertices for the quad
    setCanvas();
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Clear the screen
    glClearColor(0.086f, 0.086f, 0.086f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // pixels = new GLubyte[tilesX*tilesY*3];
    std::cout << offset.x << '\n';
    lvl->draw(pixels);
    drawTiles2();
    // delete[] pixels;
    if(stg.scale > 20)
        drawGrid();

    SDL_GL_SwapWindow(window);

    
}


int main()
{
    getScreenSize();
    SDL_CreateWindowAndRenderer(screen_width, screen_height, 0, &window, nullptr);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    initGL(window);

    Grid grid = Grid(stg.tilesX, stg.tilesY, stg);
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
            (rand() % 16) == 15
        ));
    }
    std::cout << lvl->tiles[144]->x << '\n';

    emscripten_set_main_loop(main_loop, 0, true);

    return EXIT_SUCCESS;
}
