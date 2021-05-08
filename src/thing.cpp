#include "headers/thing.hpp"
#include "headers/globals.hpp"


Thing::Thing(int x, int y, TileType type) : type(type), pos(x,y)
{
}

Thing::~Thing()
{
	tiles.clear();
}