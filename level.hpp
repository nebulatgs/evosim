#pragma once

#include <vector>
#include <stdint.h>

#include "tile.hpp"

class Level {
public:
    Level();
    ~Level();
    void add(Food* food);
    void add(Creature* creature);
    void add(Border* border);
    void update(uint8_t *pixels);
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

    long oldTime;
    bool antibiotic;
};