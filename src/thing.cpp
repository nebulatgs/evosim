#include "headers/thing.hpp"
#include "headers/globals.hpp"

void Thing::randomize()
{
	pos.x = (rand() % (stg.map_width - 2)) + 1;
	pos.y = (rand() % (stg.map_height - 2)) + 1;
}



Thing::Thing(int x, int y, TileType type) : type(type)
{
	pos.x = x;
	pos.y = y;
}

Thing::~Thing()
{
	tiles.clear();
}