
#include "headers/border.hpp"

Border::Border(int x, int y) : Thing(x, y, TileType::Wall)
{
	tiles.push_back(Tile(x, y, {0, 0}, color));
}

bool Border::update(uint8_t *pixels, Level *lvl)
{
	for (auto x : tiles)
	{
		x.update(pixels, color);
	}
	return 0;
}