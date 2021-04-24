#pragma once

#include <stdint.h>

#include "v2d.h"
#include "gene.hpp"


enum class TileType
{
	Wall,
	Food,
	Creature
};

// Represents a single tile on the world grid
class Tile
{
public:
	Tile(int x, int y, v2d offset, uint32_t color);
	virtual void update(uint8_t *pixels, uint32_t color);
	int x, y;
	v2d offset;
	TileType type;
	uint32_t color;
};
