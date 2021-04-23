#include <iostream>
#include "tile.hpp"
#include "level.hpp"
#include "proteins.hpp"
#include "globals.hpp"

enum class mutationType
{
	pSub,
	pIns,
	pDel,
	gInv,
	gDel
};

extern std::vector<std::vector<Gene>> ecosystem;
uint32_t oldCol = 0x3C4CE7;

std::string Gene::toString()
{
	std::string concat;
	for (auto x : bases)
	{
		char base;
		switch (x)
		{
		case 0:
			base = 'A';
			break;

		case 1:
			base = 'T';
			break;

		case 2:
			base = 'G';
			break;

		case 3:
			base = 'C';
			break;

		default:
			base = 'C';
			break;
		}
		concat.push_back(base);
	}
	return concat;
}

int Creature::digitConcat(std::vector<int> digits)
{
	int sum = digits[0];
	for (int i = 1; i < digits.size(); i++)
	{
		int magnitude = 1;
		while (magnitude <= digits[i])
			magnitude *= 10;
		sum = magnitude * sum + digits[i];
	}
	return sum;
}

extern bool randDensity(int number);
// bool randDensity(int number){
//     return (rand() % number) == number/2;
// }

Creature::Creature(int x, int y, int species) : Thing(x, y, TileType::Creature), species(species), size(1)
{
	// genome = ecosystem[species];
	tiles.push_back(Tile(pos.x, pos.y, {0, 0}, color));
	genome.clear();
	genome = getDefaults();
}

int Creature::readNext()
{
	std::string key = genome[geneIndex].toString();
	// std::cout << key << '\n';
	int protein = 0;
	transcribedProteins.push_back(protein);
	return transcribeIndex += 3;
}

void Creature::reset()
{
	transcribedProteins.clear();
}

bool Creature::findFood(int radius, void *buffer)
{
	// std::cout << radius << '\n';
	int index = 0;
	// v2d distance = v2d(99999,99999);
	float distance = 99999;
	int food = -1;
	// v2d vision = v2d(radius, radius);
	// int r = rand() % current_lvl->things.size() / 2;
	int r = 0;
	int d = r + current_lvl->getFoodCount() / 1;
	// int d = r - 500 > 0 ? r - 500 : r / 10;
	// int d = r / 1000;
	for (int i = r; i < d; i++)
	{
		auto thing = current_lvl->getFood(i);
		if (thing->type == TileType::Food)
		{
			if (pos.x == thing->pos.x && pos.y == thing->pos.y)
			{
				return 0;
			}
			// v2d dist = v2d(abs(thing->pos.x - pos.x),abs(thing->pos.y - pos.y));
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
	// if(closest_food->pos == foodPos){
	//     if(pos.sqrDist(foodPos) > pos.sqrDist(current_lvl->things[food]->pos) - 100)
	*(Food **)buffer = (Food *)(current_lvl->getFood(food));
	// else
	//    *(Food**)buffer = (Food*)closest_food;
	// }
	// else{
	//     *(Food**)buffer = (Food*)(current_lvl->things[food]);
	// }
	// *(Thing**)buffer = search[index];
	return 0;
}

bool Creature::moveWander()
{
	float fac = 0.5 * stg.speed;
	if (wanderTime != time(nullptr))
	{
		wanderDirection = v2d(1, 1);
		float r = ((float)rand() / (float)RAND_MAX);
		wanderDirection = (wanderDirection.rotate(r * M_PI * 2));
		wanderDirection.setLen(fac);
		// tempX = currentDirection->x, tempY = currentDirection->y;
		wanderTime = time(nullptr);
	}
	wanderDirection.setLen(fac);
	float tempX = wanderDirection.x, tempY = wanderDirection.y;
	pos.x += (tempX + pos.x) < stg.map_width - 1 && (tempX + pos.x) > 1 ? tempX : 1;
	pos.y += (tempY + pos.y) < stg.map_height - 1 && (tempY + pos.y) > 1 ? tempY : 1;
	energy -= 2 * stg.speed;
	return 0;
}

bool Creature::moveFood()
{
	if (closest_food == nullptr)
		return 1;
	v2d dist = closest_food->pos - pos;
	dist.setLen(0.5 * stg.speed);
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
		// mutate();
		Creature *child = new Creature(pos.x, pos.y, species);
		v2d childDir = v2d(1, 1);
		childDir.rotate(((float)rand() / (float)RAND_MAX) * M_PI * 2);
		childDir.setLen(size);
		child->pos += childDir / 2;
		pos -= childDir / 2;
		child->genome = genome;
		child->mutate();
		// uint8_t* childColor = (uint8_t*)(&child->color);
		// childColor[1] = (rand() % 256);
		child->found_food = 0;
		current_lvl->add(child);
	}
	return 0;
}

bool Creature::SetARes(int strength)
{
	a_res = strength;
	uint8_t *startCol = (uint8_t *)&oldCol;
	uint8_t r = (startCol[0] * (100 - strength) + (0x34 * strength)) / 100;
	uint8_t g = (startCol[1] * (100 - strength) + (0x98 * strength)) / 100;
	uint8_t b = (startCol[2] * (100 - strength) + (0xdb * strength)) / 100;
	startCol = (uint8_t *)&color;
	startCol[0] = r;
	startCol[1] = g;
	startCol[2] = b;
	// std::cout << strength << '\n';
	// #3498db
	// color = 0xb98029;
	return 0;
}

void Creature::mutate()
{
	// if(((float)rand() / (float)RAND_MAX) * 1000 * (1/stg.speed) > (1000 * (1/stg.speed) - 1)){
	//     int i = (int)((float)rand() / (float)RAND_MAX) * genome.size();
	//     int j = (int)((float)rand() / (float)RAND_MAX) * genome[i].bases.size();
	//     if(proteinLUT[genome[i].toString()] < Protein::instr_sep)
	//         genome[i].bases[j] = (int)((float)rand() / (float)RAND_MAX) * 33;
	//     else
	//         genome[i].bases[j] = (int)((float)rand() / (float)RAND_MAX) * 3;
	// // }
	if (rand() % 100 > 75)
	{
		int r = rand() % genome.size();
		if (!(proteinLUT[genome[r].toString()] > Protein::instr_sep))
			genome[r].bases[rand() % 3] = rand() % 33;
	}
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
		// closest_food -> food = 0;
		found_food = false;
		energy += 250;
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
		// std::cout << _size << "\n";
	}
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
		// error = 0;
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
	// if(result == -1)
	//     return -1;
	return error;
}

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

bool Creature::processGenome()
{
	using namespace std;
	stack<int> brain;
	for (int i = 0; i < genome.size(); i++)
	{
		int protein = proteinLUT[genome[i].toString()];
		// void* instruction;
		bool (*instuction)(int, void *);
		if (protein > Protein::instr_sep)
		{
			// Process instruction
			if (brain.empty())
			{
				if (ltbrain.empty())
					return 1;
				int top = ltbrain.top();
				ltbrain.pop();
				ltbrain.push(processInstruction(protein, top));
			}
			else
			{
				if (!ltbrain.empty())
					ltbrain.pop();
				int top = brain.top();
				brain.pop();
				ltbrain.push(processInstruction(protein, top));
			}
		}
		else
		{
			// Process value
			i++;
			brain.push(accumulate(genome[i].bases.begin(), genome[i].bases.end(), 0));
		}
	}
	return 0;
}

bool Creature::update(uint8_t *pixels, Level *lvl)
{
	// if(!found_food){
	//     closest_food = nullptr;
	//     distance.set(1000000,1000000);
	//     int index = 0;

	//     if(closest_food == nullptr && index == 0)
	//         return 1;
	//     else if(closest_food == nullptr){
	//         wander = 1;
	//     }
	//     else {
	//         wander = 0;
	//     }
	//     found_food = 1;
	// }
	// Move

	// Reproduce at sufficient energy

	// Eat food

	// Draw me
	current_lvl = lvl;
	// genome = getDefaults();
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
		// if(((float)rand() / (float)RAND_MAX) * a_res < 1){
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
	// Am I dead?
	// if(closest_food == nullptr && !wander)
	//     return 1;
	return !energy;
}

Food::Food(int x, int y, bool food) : Thing(x, y, TileType::Food), food(food)
{
	if (food)
	{
		tiles.push_back(Tile(x, y, {0, 0}, color));
	}
}

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

void Thing::randomize()
{
	pos.x = (rand() % (stg.map_width - 2)) + 1;
	pos.y = (rand() % (stg.map_height - 2)) + 1;
}

bool Food::update(uint8_t *pixels, Level *lvl)
{
	if (food)
	{
		for (auto tile : tiles)
		{
			// x = !(bool)(rand() % 50) ? (rand() % stg.map_width) : x;
			// y += 1;
			// randomize();
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

Thing::Thing(int x, int y, TileType type) : type(type)
{
	pos.x = x;
	pos.y = y;
}

Thing::~Thing()
{
	tiles.clear();
}

Creature::~Creature()
{
	tiles.clear();
	transcribedProteins.clear();
	genome.clear();
}