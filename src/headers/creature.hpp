#pragma once

#include <vector>
#include <stdint.h>

#include "thing.hpp"

class Food;

class Creature : public Thing
{
public:
	Creature(int x, int y, int species);
	~Creature();

	int mutate(int count, mutationType mutation);
	bool update(uint8_t *pixels, Level *lvl);
	int readNext();
	void reset();

protected:
	bool processGenome();
	int processInstruction(int protein, int memory);
	bool findFood(int radius, void *buffer);
	bool moveFood();
	bool eatFood();
	bool reproduce();
	bool moveWander();
	bool SetARes(int strength);
	bool SetSize(int _size);
	void mutate();
	std::vector<Gene> getDefaults();

private:
	void pSub();
	void pIns();
	int digitConcat(std::vector<int> digits);

private:
	bool eaten = 0;
	double eatTime;
public:
	Level *current_lvl = nullptr;

	TileType type = TileType::Creature;
	uint32_t size;
	
	constexpr static uint32_t origCol = 0xa879fd;
	uint32_t color = 0xa879fd;
	// uint32_t oldCol = 0x3C4CE7;
	
	float energy = 250;
	std::vector<Gene> genome;
	int species;
	int a_res = 0;
	float speed = 1.0f;

	bool wander;
	v2d wanderDirection;
	long wanderTime;

	Food *closest_food = nullptr;
	bool found_food = false;
	v2d foodPos = v2d(0, 0);
	v2d distance = v2d(999999, 999999);

	uint32_t geneIndex;
	std::vector<int> ltbrain;
};
