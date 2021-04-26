#include <time.h>

#include "headers/creature.hpp"
#include "headers/globals.hpp"
#include "headers/level.hpp"
#include "headers/proteins.hpp"

std::vector<Gene> Creature::getDefaults()
{
	return {
		{{2, 0, 0}},
		{{9, 9, 9}},
		{{0, 0, 0}},
		
		{{0, 1, 0}},
		{{0, 2, 0}},
		{{0, 3, 0}},

		{{1, 0, 1}}, //Read antibiotic resistance gene
		{{5, 5, 5}},
		{{1, 0, 2}},

		{{1, 0, 3}}, // Read size gene
		{{5, 5, 5}},
		{{1, 1, 0}}};
}

bool Creature::update(uint8_t *pixels, Level *lvl)
{
	current_lvl = lvl;
	if (processGenome())
	{
		return 1;
	}
	for (auto tile : tiles)
	{
		if (pos.x + tile.offset.x > stg.map_width ||
			pos.x + tile.offset.x < 0 ||
			pos.y + tile.offset.y > stg.map_height ||
			pos.y + tile.offset.y < 0)
		{
			continue;
		}
		tile.x = pos.x + tile.offset.x;
		tile.y = pos.y + tile.offset.y;
		tile.update(pixels, color);
	}
	if (current_lvl->isAntibiotic())
	{
		if (reverse_a)
		{
			if ((rand() % 100) < a_res)
				return 1;
		}
		else
		{
			if (!((rand() % 100) < a_res))
				return 1;
		}
	}
	return !energy;
}

bool Creature::processGenome()
{
	using namespace std;
	stack<int> brain;
	for (int i = 0; i < genome.size(); i++)
	{
		int protein = proteinLUT[genome[i].toString()];
		bool (*instuction)(int, void *);
		if (protein > Protein::instr_sep)
		{
			// Process instruction
			if (brain.empty())
			{
				if (ltbrain.empty())
					return 1;
				int top = ltbrain.back();
				ltbrain.pop_back();
				ltbrain.push_back(processInstruction(protein, top));
			}
			else
			{
				if (!ltbrain.empty())
					ltbrain.pop_back();
				int top = brain.top();
				brain.pop();
				ltbrain.push_back(processInstruction(protein, top));
			}
		}
		else
		{
			// Process value
			i++;
			brain.push(accumulate(genome[i].bases.begin(), genome[i].bases.end(), 0));
		}
	}
	ltbrain.clear();

	return 0;
}

int Creature::processInstruction(int protein, int memory)
{
	Food *result;
	bool error;
	switch (protein)
	{
	case FindFood:
		error = findFood(memory, &result);
		if (error)
			return -1;
		closest_food = result;
		foodPos = closest_food->pos;
		break;
	case MoveFood:
		if (memory > -1)
			error = moveFood();
		else
			error = moveWander();
		break;
	case EatFood:
		eatFood();
		break;
	case Reproduce:
		reproduce();
		break;
	case SetA_Res:
		SetARes(memory);
	case Set_Size:
		SetSize(memory);
	default:
		error = 1;
	}
	return error;
}

Creature::Creature(int x, int y, int species) : Thing(x, y, TileType::Creature), species(species), size(1)
{
	tiles.push_back(Tile(pos.x, pos.y, {0, 0}, color));
	genome.clear();
	genome = getDefaults();
}

bool Creature::findFood(int radius, void *buffer)
{
	if (eatTime > emscripten_get_now())
	{
		return 1;
	}
	int index = 0;
	float distance = 99999;
	int food = -1;
	int r = 0;
	int d = r + current_lvl->getFoodCount() / 1;
	for (int i = r; i < d; i++)
	{
		auto thing = current_lvl->getFood(i);
		if (thing->type == TileType::Food)
		{
			if (pos.x == thing->pos.x && pos.y == thing->pos.y)
			{
				return 0;
			}
			float dist = pos.sqrDist(thing->pos);
			if (dist < distance && dist < radius * radius && ((Food *)thing)->food)
			{
				distance = dist;
				food = index;
			}
		}
		index++;
	}
	if (food == -1)
		return 1;

	*(Food **)buffer = (Food *)(current_lvl->getFood(food));

	return 0;
}

bool Creature::moveWander()
{
	float fac = 0.5 * stg.speed * speed;
	if (wanderTime != time(nullptr))
	{
		wanderDirection = v2d(1, 1);
		float r = ((float)rand() / (float)RAND_MAX);
		wanderDirection = (wanderDirection.rotate(r * M_PI * 2));
		wanderDirection.setLen(fac);
		wanderTime = time(nullptr);
	}
	wanderDirection.setLen(fac);
	float tempX = wanderDirection.x, tempY = wanderDirection.y;
	pos.x += (tempX + pos.x) < stg.map_width - 1 && (tempX + pos.x) > 1 ? tempX : 1;
	pos.y += (tempY + pos.y) < stg.map_height - 1 && (tempY + pos.y) > 1 ? tempY : 1;
	if (eatTime > emscripten_get_now())
	{
		energy -= 0.5 * stg.speed;
		return 0;
	}
	energy -= 2 * stg.speed;
	return 0;
}

bool Creature::moveFood()
{
	if (closest_food == nullptr)
		return 1;
	v2d dist = closest_food->pos - pos;
	dist.setLen(0.5 * stg.speed * speed);
	float tempX = dist.x, tempY = dist.y;
	pos.x += (tempX + pos.x) < stg.map_width - 1 && (tempX + pos.x) > 1 ? tempX : 1;

	pos.y += (tempY + pos.y) < stg.map_height - 1 && (tempY + pos.y) > 1 ? tempY : 1;
	energy -= 2 * stg.speed;
	return 0;
}

bool Creature::reproduce()
{
	if (energy > 500)
	{
		energy -= 250;
		Creature *child = new Creature(pos.x, pos.y, species);
		v2d childDir = v2d(1, 1);
		childDir.rotate(((float)rand() / (float)RAND_MAX) * M_PI * 2);
		childDir.setLen(size);
		child->pos += childDir / 2;
		pos -= childDir / 2;
		child->genome = genome;
		child->mutate();
		child->found_food = 0;
		current_lvl->add(child);
	}
	return 0;
}

bool Creature::SetARes(int strength)
{
	a_res = strength;
	uint8_t *startCol = (uint8_t *)&origCol;
	uint8_t r = (startCol[0] * (100 - strength) + (0x34 * strength)) / 100;
	uint8_t g = (startCol[1] * (100 - strength) + (0x98 * strength)) / 100;
	uint8_t b = (startCol[2] * (100 - strength) + (0xdb * strength)) / 100;
	startCol = (uint8_t *)&color;
	startCol[0] = r;
	startCol[1] = g;
	startCol[2] = b;
	return 0;
}

void Creature::mutate()
{
	// if (rand() % 100 > 75)
	// {
	int r = rand() % genome.size();
	if (!(proteinLUT[genome[r].toString()] > Protein::instr_sep))
		genome[r].bases[rand() % 3] = rand() % 33;
	// }
}

bool Creature::eatFood()
{
	if (closest_food == nullptr)
		return 1;
	if (abs(pos.x - closest_food->pos.x) <= size / 2 &&
		abs(pos.y - closest_food->pos.y) <= size / 2 &&
		closest_food->food)
	{
		closest_food->randomize();
		found_food = false;
		energy += 250;
		eaten = true;
		eatTime = emscripten_get_now() + 1500 / (size * speed);
	}
	if (closest_food->pos != foodPos)
	{
		found_food = false;
	}
	return 0;
}

bool Creature::SetSize(int _size)
{
	if (_size != size)
	{
		size = (_size >> 4) + 2;
		speed = 2 / (float)size;
		tiles.clear();
		int r = size >> 1;
		for (float ty = 0; ty < 2.0 * r; ty++)
		{
			for (float tx = 0; tx < 2 * r; tx++)
			{
				float deltaX = r - tx;
				float deltaY = r - ty;
				float distance = sqrt(deltaX * deltaX + deltaY * deltaY);
				float col = r - distance;
				if (col > 0)
					tiles.push_back(Tile(pos.x, pos.y, v2d(tx - r, ty - r), color));
			}
		}
	}
	return 0;
}

Creature::~Creature()
{
	tiles.clear();
	genome.clear();
}