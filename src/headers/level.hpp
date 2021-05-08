#pragma once

#include <vector>
#include <stdint.h>

#include "thing.hpp"
#include "food.hpp"
#include "creature.hpp"
#include "border.hpp"


class Level {
public:
    Level();
    ~Level();
    void add(Food* food);
    void add(Creature* creature);
    void add(Border* border);
	void add(Species *species);
    void update(uint8_t *pixels);

	float getCreatureAvgResistance();
	float getCreatureAvgSize();
	int getCreatureCount();
	int getTotalDeaths();

    int getFoodCount();
    Thing* getFood(int i);
	Food* getFood(int x, int y);
	void setFood(int x, int y, Food* value);
    bool isAntibiotic();

private:
    template<typename T>
    void update(std::vector<T>& things);
    
private:
    std::vector<Food*> foods;
    std::vector<Creature*> creatures;
    std::vector<Border*> borders;
	std::vector<Species*> species;
	Food** foodMap;
    double oldTime;
	int deaths = 0;
    bool antibiotic;
};