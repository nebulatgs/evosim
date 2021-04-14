#include "tile.hpp"
#include <iostream>
// extern int tilesX, tilesY;
enum class mutationType {pSub, pIns, pDel, gInv, gDel};
enum class TileType {Air, Food, Creature};
extern std::vector<std::vector<Gene>> ecosystem;

struct Settings{
    uint16_t screen_width, screen_height;
    const uint16_t map_width, map_height;
    const uint16_t tilesX, tilesY;
    uint16_t init_width, init_height;
    float scale, drag;
};
extern Settings stg;
// Contains a vector of bases which are read 3 at a time (ex. ACG) to form proteins

std::string Gene::toString(){
    std::string concat;
    for(auto x : bases){
        char base;
        switch (x){
            case 0: base = 'A'; break;
            
            case 1: base = 'T'; break;

            case 2: base = 'G'; break;

            case 3: base = 'C'; break;

            default: base = 'C'; break;
        }
        concat.push_back(base);
    }
    return concat;
}


int Creature::digitConcat(std::vector<int> digits){
    int sum = digits[0];
    for(int i = 1; i < digits.size(); i++){
        int magnitude = 1;
        while(magnitude <= digits[i]) magnitude *= 10;
        sum = magnitude * sum + digits[i];
    }
    return sum;
}


Creature::Creature(int x, int y, int species) : Thing(x, y), species(species), size(1){
    genome = ecosystem[species];
}
int Creature::readNext(){
    std::string key = genome[geneIndex].toString();
    // std::cout << key << '\n';
    int protein = 0;
    transcribedProteins.push_back(protein);
    return transcribeIndex+=3;
}
void Creature::reset(){
    transcribedProteins.clear();
}

Resource::Resource(int x, int y, bool food):
    Thing(x,y),
    food(food){
        if(food)
            tiles.push_back(Tile(x, y, color));
}

Border::Border(int x, int y) : Thing(x, y){
    tiles.push_back(Tile(x, y, color));
}

void Border::update(uint8_t *pixels){
    for(auto x : tiles){
        x.update(pixels);
    }
}

void Resource::update(uint8_t *pixels){
    if(food)
        for(auto x : tiles){
            x.update(pixels);
        }
}

void Tile::update(uint8_t *pixels){
    int index = x * 3 + (y * 3 * stg.map_width);
    pixels[index++] = ((uint8_t*)&color)[0];
    pixels[index++] = ((uint8_t*)&color)[1];
    pixels[index++] = ((uint8_t*)&color)[2];
}
Tile::Tile(int x, int y, uint32_t color):
    x(x),
    y(y),
    color(color){
}
Thing::Thing(int x, int y):
x(x),
y(y){    
}