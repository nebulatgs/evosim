#include "globals.hpp"
#pragma once


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
    // screen_width = screen_width > 1920 ? 1920  : screen_width;
    // screen_height = screen_height > 1080 ? 1080 : screen_height;
}

// GLfloat vertices[12] = {-1.0f,  1.0f,
//                         -1.0f, -1.0f,
//                          1.0f, -1.0f,
                         
//                         -1.0f, -1.0f,
//                          1.0f,  1.0f,
//                          1.0f, -1.0f};

GLfloat vertices[12] = {-1.0f,  1.0f,
                        -1.0f, -1.0f,
                         1.0f, 1.0f,
                    
                         1.0f, 1.0f,
                         1.0f, -1.0f,
                         -1.0f, -1.0f};

GLfloat quadVertices[] = {
    // positions     // colors
    -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
     0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
    -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

    -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
     0.05f, -0.05f,  0.0f, 1.0f, 0.0f,   
     0.05f,  0.05f,  0.0f, 1.0f, 1.0f		    		
};  


// Shader sources
static std::string read_shader_file
(
    const std::__fs::filesystem::path::value_type *shader_file)
{
    std::ifstream ifs;

    auto ex = ifs.exceptions();
    ex |= std::ios_base::badbit | std::ios_base::failbit;
    ifs.exceptions(ex);

    ifs.open(shader_file);
    ifs.ignore(std::numeric_limits<std::streamsize>::max());
    auto size = ifs.gcount();

    // if (size > 0x10000) // 64KiB sanity check:
    //     return false;

    ifs.clear();
    ifs.seekg(0, std::ios_base::beg);

    return std::string {std::istreambuf_iterator<char> {ifs}, {}};
}



// an example of something we will control from the javascript side
bool background_is_black = true;

void handleEvents(float *zoomPhysics, v2d *panPhysics, float scaleClipStart, float scaleClipEnd){
    SDL_Event event;
    while( SDL_PollEvent( &event ) ){
    // std::cout << event.type << '\n';
        switch (event.type){
            case SDL_MOUSEWHEEL:
                // stg.scale += event.wheel.y > 0 ? (float)event.wheel.y / 2000.0f : (float)event.wheel.y / 2000.0f;
                // zoomPhysics[0] += zoomPhysics[2] > scaleClipEnd || zoomPhysics[2] < scaleClipStart ? copysign(1.0, event.wheel.y) * 0.001 * -1.0 : ((float)event.wheel.y / 16000.0f);
                // if(zoomPhysics[2] > scaleClipEnd || zoomPhysics[2] < scaleClipStart){
                //     zoomPhysics[0] = 0;
                //     zoomPhysics[1] = 0;
                //     zoomPhysics[2] += copysign(1.0, event.wheel.y) * 10;
                // }
                // else
                    zoomPhysics[0] += ((float)event.wheel.y / 16000.0f);
                break;
            case SDL_MULTIGESTURE:
                zoomPhysics[0] += ((float)event.mgesture.dDist / 10);
                break;
            case SDL_MOUSEBUTTONDOWN:
                isMouseDown = true;
                SDL_GetRelativeMouseState(nullptr, nullptr);
                break;
            case SDL_MOUSEBUTTONUP:
                isMouseDown = false;
                break;
        }
    }
    if(isMouseDown){
        int x,y;
        SDL_GetRelativeMouseState(&x, &y);
        panPhysics[2] += v2d(-x,y);
        // panPhysics[1] += v2d(-event.motion.xrel, event.motion.yrel)/100000;
    }
}
float* processPhysics(float *physics, float drag, float clipStart, float clipEnd){

    float temp = physics[2];
    physics[1] += physics[0];
    physics[0] /= drag;

    
    // Clip Position
    physics[2] = physics[2] > clipEnd ? clipEnd : physics[2];
    
    // pos += vel
    float adjVel = physics[1] * physics[2];
    physics[2] += adjVel;
    physics[1] /= drag;

    if(physics[2] < clipStart){
        physics[0] = 0;
        physics[1] = 0;
        physics[2] = clipStart;
    }

    return physics;
}

v2d* processPhysics(v2d *physics, v2d drag, v2d clipStart, v2d clipEnd){

    v2d temp = physics[2];
    physics[1] += physics[0];
    physics[0] /= drag;

    
    // Clip Position
    physics[2].x = physics[2].x > clipEnd.x ? clipEnd.x : physics[2].x;
    physics[2].y = physics[2].y > clipEnd.y ? clipEnd.y : physics[2].y;

    // pos += vel
    v2d adjVel = physics[1] * physics[2];
    physics[2] += adjVel;
    physics[1] /= drag;
    
    if(physics[2].x < clipStart.x){
        physics[0].x = 0;
        physics[1].x = 0;
        physics[2].x = clipStart.x;
    }
    if(physics[2].y < clipStart.y){
        physics[0].y = 0;
        physics[1].y = 0;
        physics[2].y = clipStart.y;
    }

    return physics;
}
// the function called by the javascript code
extern "C" void EMSCRIPTEN_KEEPALIVE toggle_background_color() { background_is_black = !background_is_black; }
void setCanvas(){
    getScreenSize();
    SDL_SetWindowSize(window, (int)screen_width, (int)screen_height);
    SDL_FlushEvent(SDL_WINDOWEVENT);
}

void drawGrid(){
        glUseProgram(gridProgram);
    GLint uniform_Resolution = glGetUniformLocation(gridProgram, "resolution");
        glUniform2f(uniform_Resolution, screen_width, screen_height);

    GLint uniform_Scale = glGetUniformLocation(gridProgram, "scale");
        glUniform1f(uniform_Scale, stg.scale);

    GLint uniform_Offset = glGetUniformLocation(gridProgram, "offset");
        glUniform2f(uniform_Offset, offset.x, offset.y);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

    // Draw a triangle fan for the quad

}

void drawTiles(){
    glUseProgram(tilesProgram);
    GLint uniform_ScaleTiles = glGetUniformLocation(tilesProgram, "scale");
        glUniform1f(uniform_ScaleTiles, (1/stg.scale)*504.8);
        
    GLint uniform_OffsetTiles = glGetUniformLocation(tilesProgram, "offset");
        glUniform2f(uniform_OffsetTiles, offset.x, offset.y);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
}

void drawTiles2(){
    constexpr int power = 10;
    // int tilesX = (screen_width/scale);
    // int tilesY = screen_height/scale;
    constexpr int tilesX = ipow(2, power);
    constexpr int tilesY = ipow(2, power);

    // auto pixels = new GLubyte[tilesX*tilesY*3];
    //auto pixels = new GLubyte[tilesX*tilesY*3];
    // GLubyte*** pixels = new GLubyte**[tilesX];
    // for(int i = 0; i < tilesX; ++i)
    //     pixels[i] = (GLubyte**)new GLubyte[tilesY][3];
    //     // pixels[i][0][0] = (GLubyte) i%255;
        // pixels[i][0][1] = (GLubyte) i%255;
        // pixels[i][0][2] = (GLubyte) i%255;
    // auto pixels = new GLubyte[tilesX * tilesY * 3];
       
    // std::cout << (int)pixels[2][2][2] << '\n';
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tilesX, tilesY, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    
    glUseProgram(tiles2Program);
    GLint uniform_Resolution = glGetUniformLocation(tiles2Program, "resolution");
        glUniform2f(uniform_Resolution, screen_width, screen_height);
    GLint uniform_ScaleTiles = glGetUniformLocation(tiles2Program, "scale");
        glUniform1f(uniform_ScaleTiles, stg.scale);
        
    GLint uniform_OffsetTiles = glGetUniformLocation(tiles2Program, "offset");
        glUniform2f(uniform_OffsetTiles, offset.x, offset.y);

    GLint uniform_tileDims = glGetUniformLocation(tiles2Program, "tileDims");
        glUniform2f(uniform_tileDims, tilesX, tilesY);
    GLint uniform_texture = glGetUniformLocation(tiles2Program, "texture");
        glUniform1i(uniform_texture, 0);
    // std::cout << stg.scale << '\n';
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
    // for (int i = 0; i < tilesX; ++i)
    // {
    //     delete[](pixels[i]);
    // }
    // delete[] pixels;
    
    glDeleteTextures(1, &textureID);
} 

GLuint compileShaders(std::string vertShader, std::string fragShader){
    GLchar *gridVertexSource = (char*)vertShader.c_str();
    GLchar *gridFragmentSource = (char*)fragShader.c_str();

    // std::cout << gridVertexSource << "\n";
    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &gridVertexSource, nullptr);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &gridFragmentSource, nullptr);
    glCompileShader(fragmentShader);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    return shaderProgram;
}

void initGL(SDL_Window *window){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto gridVertexSourceStr = read_shader_file("shaders/grid.vert");
    auto gridFragmentSourceStr = read_shader_file("shaders/grid.frag");
    gridProgram = compileShaders(gridVertexSourceStr, gridFragmentSourceStr);

    auto tilesVertexSourceStr = read_shader_file("shaders/tiles.vert");
    auto tilesFragmentSourceStr = read_shader_file("shaders/tiles.frag");
    tilesProgram = compileShaders(tilesVertexSourceStr, tilesFragmentSourceStr);
    
    auto tiles2VertexSourceStr = read_shader_file("shaders/texture.vert");
    auto tiles2FragmentSourceStr = read_shader_file("shaders/texture.frag");
    tiles2Program = compileShaders(tiles2VertexSourceStr, tiles2FragmentSourceStr);
    
    glUseProgram(gridProgram);
    // Specify the layout of the vertex data
    GLint posAttribGrid = glGetAttribLocation(gridProgram, "position");
    glEnableVertexAttribArray(posAttribGrid);
    glVertexAttribPointer(posAttribGrid, 2, GL_FLOAT, GL_FALSE, 0, 0);

    GLint uniform_WindowSizeGrid = glGetUniformLocation(gridProgram, "resolution");
    glUniform2f(uniform_WindowSizeGrid, screen_width, screen_height);

    glUseProgram(tilesProgram);

    GLint posAttribTiles = glGetAttribLocation(tilesProgram, "position");
    glEnableVertexAttribArray(posAttribTiles);
    glVertexAttribPointer(posAttribTiles, 2, GL_FLOAT, GL_FALSE, 0, 0);

    GLint uniform_WindowSizeTiles = glGetUniformLocation(tilesProgram, "resolution");
    glUniform2f(uniform_WindowSizeTiles, screen_width, screen_height);

    GLint uniform_ScaleTiles = glGetUniformLocation(tilesProgram, "scale");
    glUniform1f(uniform_ScaleTiles, stg.scale);
    
}