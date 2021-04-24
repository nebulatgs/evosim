#pragma once

#include "thing.hpp"

class Level;

class Border : public Thing
{
public:
	Border(int x, int y);
	bool update(uint8_t *pixels, Level *lvl);

public:
	TileType type = TileType::Wall;
	uint32_t color = 0xFF926E5F;
};
