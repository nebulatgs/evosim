#pragma once

#include <filesystem>

#include "globals.hpp"

extern GLfloat vertices[12];

static std::string read_shader_file(const std::__fs::filesystem::path::value_type *shader_file);

void handleEvents(v2d *cursor, float *zoomPhysics, v2d *panPhysics, float scaleClipStart, float scaleClipEnd);

float *processPhysics(float *physics, float drag, float clipStart, float clipEnd);

v2d *processPhysics(v2d *physics, v2d drag, v2d clipStart, v2d clipEnd);

void setCanvas();

void drawGrid();

void drawTiles();

GLuint compileShaders(std::string vertShader, std::string fragShader);

void initGL(SDL_Window *window);
