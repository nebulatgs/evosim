#include <iostream>
#include "tile.hpp"
#include "level.hpp"
#include "globals.hpp"

void Tile::update(uint8_t *pixels, uint32_t color)
{
	int index = x * 3 + (y * 3 * stg.map_width);
	if (index > stg.map_height * stg.map_width * 3)
	{
		return;
	}
	pixels[index++] = ((uint8_t *)&color)[0];
	pixels[index++] = ((uint8_t *)&color)[1];
	pixels[index++] = ((uint8_t *)&color)[2];
}

Tile::Tile(int x, int y, v2d offset, uint32_t color) : x(x), y(y), offset(offset), color(color)
{
}
