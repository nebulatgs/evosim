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
    void update(uint8_t *pixels);

	float getCreatureAvgResistance();
	float getCreatureAvgSize();
	int getCreatureCount();

    int getFoodCount();
    Thing* getFood(int i);
    bool isAntibiotic();

private:
    template<typename T>
    void update(std::vector<T>& things);
    
private:
    std::vector<Food*> foods;
    std::vector<Creature*> creatures;
    std::vector<Border*> borders;

    double oldTime;
    bool antibiotic;
};