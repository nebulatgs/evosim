#include "headers/level.hpp"
#include "headers/globals.hpp"
#include "headers/food.hpp"

void Food::randomize(Level *lvl)
{
	lvl->setFood(pos.x, pos.y, nullptr);
	pos.x = (emscripten_random() * (float)(stg.map_width - 2)) + 1;
	pos.y = (emscripten_random() * (float)(stg.map_height - 2)) + 1;
	lvl->setFood(pos.x, pos.y, this);
}
Food::Food(int x, int y, bool food) : Thing(x, y, TileType::Food), food(food)
{
	if (food)
	{
		tiles.push_back(Tile(x, y, {0, 0}, color));
	}
}

bool Food::update(uint8_t *pixels, Level *lvl)
{
	lvl->setFood(pos.x, pos.y, this);
	if (food)
	{		for (auto tile : tiles)
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