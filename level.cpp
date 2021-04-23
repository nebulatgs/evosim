#include <time.h>

#include "level.hpp"
#include "globals.hpp"
#include <iostream>

Level::Level() {
    oldTime = time(NULL) - ((6 / stg.speed) - 1);
}

void Level::add(Food* food) {
    foods.push_back(food);
}

void Level::add(Creature* creature) {
    creatures.push_back(creature);
}

void Level::add(Border* border) {
    borders.push_back(border);
}

int Level::getFoodCount() {
    return foods.size();
}

Thing* Level::getFood(int i) {
    return foods[i];
}

bool Level::isAntibiotic() {
    return antibiotic;
}

void Level::update(uint8_t *pixels){
    if(time(NULL) - oldTime > (6 / stg.speed)){
        oldTime = time(NULL);
        antibiotic = true;
        for (int i = 0; i < stg.map_height * stg.map_width * 3; i+=3){
            pixels[i] += 75;

        }
    }
    else{
        antibiotic = false;
    }
    update(foods);
    update(creatures);
    update(borders);
}

template<typename T>
void Level::update(std::vector<T>& things)
{
    for(auto iter = things.begin(); iter != things.end();)
    {
		// std::cout << (*iter)->pos.x << '\n';
		if((*iter)->pos.x == 0)
			return;
        bool death = (*iter)->update(pixels, this);
        if (death)
        {
            delete *iter;
            iter = things.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

Level::~Level(){
    for(auto food : foods){
        delete food;
    }
    for(auto creature : creatures){
        delete creature;
    }
    for(auto border : borders){
        delete border;
    }
}