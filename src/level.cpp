#include <time.h>

#include "headers/level.hpp"
#include "headers/globals.hpp"
#include <iostream>

Level::Level()
{
	oldTime = time(NULL) - ((6 / stg.speed) - 1);
}

void Level::add(Food *food)
{
	foods.push_back(food);
}

void Level::add(Creature *creature)
{
	creatures.push_back(creature);
}

void Level::add(Border *border)
{
	borders.push_back(border);
}

int Level::getFoodCount()
{
	return foods.size();
}

Thing *Level::getFood(int i)
{
	return foods[i];
}

bool Level::isAntibiotic()
{
	return antibiotic;
}

float Level::getCreatureAvgResistance()
{
	uint32_t sum = 0;
	float avg;
	for (auto creature : creatures)
	{
		sum += creature->a_res;
	}
	avg = (float)sum / (float)creatures.size();
	return avg;
}

float Level::getCreatureAvgSize()
{
	uint32_t sum = 0;
	float avg;
	for (auto creature : creatures)
	{
		sum += creature->size;
	}
	avg = (float)sum / (float)creatures.size();
	return avg;
}

int Level::getCreatureCount()
{
	return creatures.size();
}

void Level::update(uint8_t *pixels)
{
	if (time(NULL) - oldTime > (6 / stg.speed))
	{
		oldTime = time(NULL);
		antibiotic = true;
		for (int i = 0; i < stg.map_height * stg.map_width * 3; i += 3)
		{
			pixels[i] += 75;
		}
	}
	else
	{
		antibiotic = false;
	}
	update(foods);
	update(creatures);
	update(borders);
}

template <typename T>
void Level::update(std::vector<T> &things)
{
	size_t oldSize = things.size();
	for (int i = 0; i < oldSize; i++)
	{
		bool death = things[i]->update(pixels, this);
		oldSize = things.size();
		if (death)
		{
			delete things[i];
			things.erase(things.begin() + (i--));
			oldSize = things.size();
		}
	}
}

Level::~Level()
{
	for (auto food : foods)
	{
		delete food;
	}
	for (auto creature : creatures)
	{
		delete creature;
	}
	for (auto border : borders)
	{
		delete border;
	}
}