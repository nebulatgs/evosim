

#define GL_GLEXT_PROTOTYPES 1
#include "headers/level.hpp"
#include "headers/render.hpp"
#include <unistd.h>
#include <time.h>
Level *lvl;
v2d cursor = v2d(0, 0);
bool frame;
float _speed;
// float CalculateAvg(const std::list<float> &list)
// {
//     float avg = 0;
//     std::list<float>::const_iterator it;
//     for(it = list.begin(); it != list.end(); it++) avg += *it;
//     avg /= list.size();
// }
// float listAvg(const std::list<float> &list){
//     std::accumulate(list.begin(), list.end(), 0.0f) / list.size();
// }
uint64_t startTime, endTime;

#define Pr .299
#define Pg .587
#define Pb .114
void changeSaturation(double *R, double *G, double *B, double change)
{

	double P = sqrt(
		(*R) * (*R) * Pr +
		(*G) * (*G) * Pg +
		(*B) * (*B) * Pb);

	*R = P + ((*R) - P) * change;
	*G = P + ((*G) - P) * change;
	*B = P + ((*B) - P) * change;
}
extern "C" void EMSCRIPTEN_KEEPALIVE setFade(bool fade) { stg.trails = fade; }
extern "C" void EMSCRIPTEN_KEEPALIVE changePause(bool pause, float speed)
{
	stg.speed = pause ? 0 : speed;
	_speed = speed;
}
extern "C" void EMSCRIPTEN_KEEPALIVE setSpeed(float speed) { stg.speed = speed; }
extern "C" void EMSCRIPTEN_KEEPALIVE reverseA(bool a) { reverse_a = a; }
// std::cout << stg.speed << '\n';
extern "C" void EMSCRIPTEN_KEEPALIVE nextFrame(float speed)
{
	_speed = speed;
	frame = 1;
}

extern "C" float EMSCRIPTEN_KEEPALIVE getAvgResistance()
{
	return lvl->getCreatureAvgResistance();
};
extern "C" float EMSCRIPTEN_KEEPALIVE getAvgSize()
{
	return lvl->getCreatureAvgSize();
};
extern "C" int EMSCRIPTEN_KEEPALIVE getCount()
{
	return lvl->getCreatureCount();
};

int main();

extern "C" void EMSCRIPTEN_KEEPALIVE restart()
{
	emscripten_cancel_main_loop();
	memset(pixels, 0, stg.map_height * stg.map_width * 3);
	delete[] pixels;
	pixels = nullptr;
	delete lvl;
	main();
}

void main_loop()
{
	if (stg.speed == 0 && frame)
	{
		stg.speed = _speed;
	}
	// if (stg.speed == 0){
	// 	return;
	// }
	endTime = SDL_GetPerformanceCounter();
	float elapsed = (endTime - startTime) / (float)SDL_GetPerformanceFrequency();
	frameAvg.push_back(1.0f / elapsed);
	if (frameAvg.size() > 20)
		frameAvg.pop_front();
	float sum = 0;
	for (auto x : frameAvg)
	{
		sum += x;
	}
	// std::cout << "Current FPS: " << std::to_string((int)round(sum/20.0f)) << '\n';
	startTime = SDL_GetPerformanceCounter();
	handleEvents(&cursor, zoomPhysics, panPhysics, 1.0f, 50.0f);

	// Calculate zoom and pan
	float maxScale = ((float)screen_width / (float)stg.map_width) / 2.0f;
	stg.scale = processPhysics(zoomPhysics, stg.drag, maxScale, 50)[2];
	
	// v2d clipEnd = {stg.map_width * maxScale * stg.scale * 2 - (float)(stg.map_width * 2), stg.map_height * maxScale * stg.scale * 2 - (float)(stg.map_height * 2)};
	// v2d clipStart = {-(stg.map_width * maxScale * stg.scale * 2 - (float)(stg.map_width * 2)) / 2, -(stg.map_height * maxScale * stg.scale * 2 - (float)(stg.map_height * 2)) / 2};
	
	v2d clipEnd = {(float)screen_width * stg.scale - (float)screen_width, (float)screen_height * stg.scale - (float)screen_height};
	// v2d clipStart = {-(stg.map_width * maxScale * stg.scale * 2 - (float)(stg.map_width * 2)) / 2, -(stg.map_height * maxScale * stg.scale * 2 - (float)(stg.map_height * 2)) / 2};
	v2d clipStart = {0, 0};
	
	offset = processPhysics(panPhysics, {stg.drag, stg.drag}, clipStart, clipEnd)[2];

	v2d temp = cursor;
	temp.x /= screen_width / (4.0f * maxScale);
	temp.y = screen_height - temp.y;
	temp.y /= screen_height / (4.0f * maxScale);
	// temp.y += 2;
	// emscripten_log(0, "%f, %f", offset.x, offset.y);
	offset += v2d(stg.map_width / (2 * maxScale), stg.map_height / (2 * maxScale)) * v2d(stg.scale - maxScale, stg.scale - maxScale) * temp;

	panPhysics[2].x = offset.x < clipStart.x ? panPhysics[2].x - (offset.x - clipStart.x) : panPhysics[2].x;
	panPhysics[2].y = offset.y < clipStart.y ? panPhysics[2].y - (offset.y - clipStart.y) : panPhysics[2].y;

	panPhysics[2].x = offset.x > clipEnd.x ? panPhysics[2].x - (offset.x - clipEnd.x) : panPhysics[2].x;
	panPhysics[2].y = offset.y > clipEnd.y ? panPhysics[2].y - (offset.y - clipEnd.y) : panPhysics[2].y;

	offset = processPhysics(panPhysics, {stg.drag, stg.drag}, clipStart, clipEnd)[2];
	offset += v2d(stg.map_width / (2 * maxScale), stg.map_height / (2 * maxScale)) * v2d(stg.scale - maxScale, stg.scale - maxScale) * temp;

	offset.x = offset.x > clipEnd.x ? clipEnd.x : offset.x;
	offset.y = offset.y > clipEnd.y ? clipEnd.y : offset.y;


	offset.x = offset.x < clipStart.x ? 0 : offset.x;
	offset.y = offset.y < clipStart.y ? 0 : offset.y;



	// Set canvas size and buffer the vertices for the quad
	setCanvas();
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Clear the screen
	glClearColor(0.086f, 0.086f, 0.086f, 0.0f);
	// glClearColor(1.0, 0.914, 0.953, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// std::cout << offset.x << '\n';

	if (stg.speed != 0)
	{
		if (!stg.trails)
			memset(pixels, 0, stg.map_height * stg.map_width * 3);
		else
		{
			for (int i = 0; i < stg.map_height * stg.map_width * 3; i += 3)
			{
				// double b = pixels[i];
				// double g = pixels[i + 1];
				// double r= pixels[i + 2];
				// changeSaturation(&r, &g, &b, 0.9);
				// pixels[i] = b;
				// pixels[i + 1] = g;
				// pixels[i + 2] = r;
				pixels[i] = pixels[i] <= 0x27 ? 0 : pixels[i] - 10;
				pixels[i + 1] = pixels[i + 1] <= 0x27 ? 0 : pixels[i + 1] - 10;
				pixels[i + 2] = pixels[i + 2] <= 0x27 ? 0 : pixels[i + 2] * 2 - 10;

			}
		}
		lvl->update(pixels);
	}
	glGenFramebuffers(1, &fbo);
	drawTiles();

	if (stg.scale > 10)
		drawGrid();

	SDL_GL_SwapWindow(window);
	if (frame)
	{
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
	stg.map_height = screen_height / 2;
	stg.map_width = screen_width / 2;
	pixels = new GLubyte[stg.map_width * 2 * stg.map_height * 2 * 3];
	// stg.init_width = screen_width;
	// stg.init_height = screen_height;
	SDL_Renderer *renderer;
	SDL_CreateWindowAndRenderer(screen_width, screen_height, 0, &window, &renderer);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 0);
	SDL_GL_SetSwapInterval(1);

	initGL(window);

	//Grid grid = Grid(stg.map_width, stg.map_height, stg);
	lvl = new Level();

	// Push food cells
	for (int i = 0; i < stg.map_width * stg.map_height; i++)
	{
		int x = i % stg.map_width;
		x += 1;
		int y = i / stg.map_width;
		y += 1;
		// if (x == 0 || y == 0 || i % stg.map_width == stg.map_width - 1 || i / stg.map_width == stg.map_height - 1)
		// {
		// 	lvl->add(new Border(
		// 		x,
		// 		y));
		// 	continue;
		// }
		if (randDensity(1000))
		{
			lvl->add(new Food(
				x,
				y,
				1));
		}
		if (randDensity(10000))
		{
			lvl->add(new Creature(
				x,
				y,
				0));
		}
	}
	// std::cout << lvl->things[14]->x << '\n';

	emscripten_set_main_loop(main_loop, 0, true);

	return EXIT_SUCCESS;
}
