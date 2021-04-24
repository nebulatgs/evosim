#include "headers/food.hpp"

Food::Food(int x, int y, bool food) : Thing(x, y, TileType::Food), food(food)
{
	if (food)
	{
		tiles.push_back(Tile(x, y, {0, 0}, color));
	}
}

bool Food::update(uint8_t *pixels, Level *lvl)
{
	if (food)
	{
		for (auto tile : tiles)
		{
			tile.x = pos.x + tile.offset.x;
			tile.y = pos.y + tile.offset.y;
			tile.update(pixels, color);
		}
		return 0;
	}
	else
	{
		return 1;
	}
}