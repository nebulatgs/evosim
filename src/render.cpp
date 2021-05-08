#include <fstream>

#include "headers/render.hpp"

GLfloat vertices[12] = {
	-1.0f,  1.0f,
	-1.0f, -1.0f,
	 1.0f,  1.0f,

	 1.0f,  1.0f,
	 1.0f, -1.0f,
	-1.0f, -1.0f};

// Shader sources
static std::string read_shader_file(
	const std::__fs::filesystem::path::value_type *shader_file)
{
	std::ifstream ifs;

	auto ex = ifs.exceptions();
	ex |= std::ios_base::badbit | std::ios_base::failbit;
	ifs.exceptions(ex);

	ifs.open(shader_file);
	ifs.ignore(std::numeric_limits<std::streamsize>::max());
	auto size = ifs.gcount();

	ifs.clear();
	ifs.seekg(0, std::ios_base::beg);

	return std::string{std::istreambuf_iterator<char>{ifs}, {}};
}

void handleEvents(v2d *cursor, float *zoomPhysics, v2d *panPhysics, float scaleClipStart, float scaleClipEnd)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		int x, y;
		switch (event.type)
		{
		case SDL_MOUSEWHEEL:
			if (firefox)
				zoomPhysics[0] += ((float)event.wheel.y / 1000.0f);
			else
				zoomPhysics[0] += ((float)event.wheel.y / 20000.0f);

			SDL_GetMouseState(&x, &y);
			cursor->set(x, y);
			break;
		case SDL_MULTIGESTURE:
			zoomPhysics[0] += ((float)event.mgesture.dDist / 4);
			SDL_GetMouseState(&x, &y);
			cursor->set(x, y);
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
	if (isMouseDown)
	{
		int x, y;
		SDL_GetRelativeMouseState(&x, &y);
		panPhysics[2] += v2d(-x, y);
	}
}

float *processPhysics(float *physics, float drag, float clipStart, float clipEnd)
{

	float temp = physics[2];
	physics[1] += physics[0];
	physics[0] /= drag;

	// Clip Position
	physics[2] = physics[2] > clipEnd ? clipEnd : physics[2];

	float adjVel = physics[1] * physics[2];
	physics[2] += adjVel;
	physics[1] /= drag;

	if (physics[2] < clipStart)
	{
		physics[0] = 0;
		physics[1] = 0;
		physics[2] = clipStart;
	}

	return physics;
}

v2d *processPhysics(v2d *physics, v2d drag, v2d clipStart, v2d clipEnd)
{

	v2d temp = physics[2];
	physics[1] += physics[0];
	physics[0] /= drag;

	// Clip Position
	// physics[2].x = physics[2].x > clipEnd.x ? clipEnd.x : physics[2].x;
	// physics[2].y = physics[2].y > clipEnd.y ? clipEnd.y : physics[2].y;

	// physics[2].x = physics[2].x < clipStart.x ? clipStart.x : physics[2].x;
	// physics[2].y = physics[2].y < clipStart.y ? clipStart.y : physics[2].y;

	v2d adjVel = physics[1] * physics[2];
	physics[2] += adjVel;
	physics[1] /= drag;

	// if (physics[2].x < clipStart.x)
	// {
	// 	physics[0].x = 0;
	// 	physics[1].x = 0;
	// 	physics[2].x = clipStart.x;
	// }
	// if (physics[2].y < clipStart.y)
	// {
	// 	physics[0].y = 0;
	// 	physics[1].y = 0;
	// 	physics[2].y = clipStart.y;
	// }

	return physics;
}

void setCanvas()
{
	getScreenSize();
	SDL_SetWindowSize(window, (int)stg.screen_width, (int)stg.screen_height);
	SDL_FlushEvent(SDL_WINDOWEVENT);
}

void drawGrid()
{
	glUseProgram(gridProgram);
	GLint uniform_Resolution = glGetUniformLocation(gridProgram, "resolution");
	glUniform2f(uniform_Resolution, stg.screen_width, stg.screen_height);

	GLint uniform_Scale = glGetUniformLocation(gridProgram, "scale");
	glUniform1f(uniform_Scale, stg.scale);

	GLint uniform_Offset = glGetUniformLocation(gridProgram, "offset");
	glUniform2f(uniform_Offset, offset.x, offset.y);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
}

void drawTiles()
{

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, stg.map_width, stg.map_height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	glUseProgram(tiles2Program);
	GLint uniform_Resolution = glGetUniformLocation(tiles2Program, "resolution");
	glUniform2f(uniform_Resolution, stg.screen_width, stg.screen_height);
	GLint uniform_ScaleTiles = glGetUniformLocation(tiles2Program, "scale");
	glUniform1f(uniform_ScaleTiles, stg.scale);

	GLint uniform_OffsetTiles = glGetUniformLocation(tiles2Program, "offset");
	glUniform2f(uniform_OffsetTiles, offset.x, offset.y);

	GLint uniform_tileDims = glGetUniformLocation(tiles2Program, "tileDims");
	glUniform2f(uniform_tileDims, stg.map_width, stg.map_height);
	GLint uniform_texture = glGetUniformLocation(tiles2Program, "texture");
	glUniform1i(uniform_texture, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
}

GLuint compileShaders(std::string vertShader, std::string fragShader)
{
	GLchar *gridVertexSource = (char *)vertShader.c_str();
	GLchar *gridFragmentSource = (char *)fragShader.c_str();

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

void initGL(SDL_Window *window)
{
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
	glUniform2f(uniform_WindowSizeGrid, stg.screen_width, stg.screen_height);

	glUseProgram(tilesProgram);

	GLint posAttribTiles = glGetAttribLocation(tilesProgram, "position");
	glEnableVertexAttribArray(posAttribTiles);
	glVertexAttribPointer(posAttribTiles, 2, GL_FLOAT, GL_FALSE, 0, 0);

	GLint uniform_WindowSizeTiles = glGetUniformLocation(tilesProgram, "resolution");
	glUniform2f(uniform_WindowSizeTiles, stg.screen_width, stg.screen_height);

	GLint uniform_ScaleTiles = glGetUniformLocation(tilesProgram, "scale");
	glUniform1f(uniform_ScaleTiles, stg.scale);

	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}