
#include "tile.hpp"

class Level;

class Thing
{
public:
	Thing(int x, int y, TileType type);
	~Thing();
	void randomize();
	virtual bool update(uint8_t *pixels, Level *lvl);

public:
	v2d pos;
	uint32_t color;
	TileType type;
	std::vector<Tile> tiles;
};

class Food : public Thing
{
public:
	Food(int x, int y, bool food);
	bool update(uint8_t *pixels, Level *lvl);

public:
	bool food;
	TileType type = TileType::Food;
	uint32_t color = 0xFF6E9055;
};

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

public:
	int transcribeIndex;
	std::vector<int> transcribedProteins;
	bool wander;
	v2d wanderDirection;
	long wanderTime;
	Food *closest_food = nullptr;
	bool found_food = false;
	v2d distance = v2d(999999, 999999);
	v2d foodPos = v2d(0, 0);
	Level *current_lvl = nullptr;
	int a_res = 0;
	std::vector<int> ltbrain;
	uint32_t geneIndex;
	TileType type = TileType::Creature;
	std::vector<Gene> genome;
	int species;
	uint32_t size;
	uint32_t color = 0xa879fd;
	float energy = 250;
};

class Border : public Thing
{
public:
	Border(int x, int y);
	bool update(uint8_t *pixels, Level *lvl);

public:
	TileType type = TileType::Wall;
	uint32_t color = 0xFF926E5F;
};
