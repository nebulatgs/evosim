#include "tile.hpp"
#include <iostream>
// extern int tilesX, tilesY;
enum class mutationType {pSub, pIns, pDel, gInv, gDel};
enum class TileType {Wall, Resource, Creature};
extern std::vector<std::vector<Gene>> ecosystem;

class Level{
    public:
        // Level();
        std::vector<Thing*> things;
        void update(uint8_t *pixels);
        // void update();
};

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

extern bool randDensity(int number);
// bool randDensity(int number){
//     return (rand() % number) == number/2;
// }

Creature::Creature(int x, int y, int species) : Thing(x, y, TileType::Creature), species(species), size(1){
    // genome = ecosystem[species];
    tiles.push_back(Tile(x, y, color));
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
void Creature::update(uint8_t *pixels, Level* lvl){
    // if(randDensity(100)){
    //     // lvl->things.push_back(new Creature(x+1, y, species));
    // }
    if(!found_food){
        for(auto thing : lvl->things){
            if(thing->type == TileType::Resource){
                if (x == thing->x && y == thing->y){
                    // thing->randomize();
                    return;
                }
                int dist = abs(sqrt((x - thing->x) * (x - thing->x) + (y - thing->y) * (y - thing->y)));
                if(dist < distance){
                    distance = dist;
                    closest_food = thing;
                }
                // y += thing->y > 0 ? 1 : -1;
            }
        }
        found_food = true;
        std::cout << closest_food->x << '\n';
    }
    
    int tempX = (closest_food->x - x > 0 ? 1 : -1);
    x +=  (tempX + x) < stg.map_width && (tempX + x) > 0 ? tempX : 0;

    int tempY = (closest_food->y - y > 0 ? 1 : -1);
    y +=  (tempY + y)< stg.map_height && (tempY + y)> 0 ? tempY : 0;
    if(x == closest_food->x && y == closest_food->y){
        closest_food->randomize();
        found_food = false;
    }
    for(auto tile : tiles){
            // x = !(bool)(rand() % 50) ? (rand() % stg.map_width) : x;
            // y += 1;
            // randomize();
            tile.x = x;
            tile.y = y;
            tile.update(pixels);
    }
}

Resource::Resource(int x, int y, bool food):
    Thing(x,y, TileType::Resource),
    food(food){
        if(food)
            tiles.push_back(Tile(x, y, color));
}

Border::Border(int x, int y) : Thing(x, y, TileType::Wall){
    tiles.push_back(Tile(x, y, color));
}

void Border::update(uint8_t *pixels, Level* lvl){
    for(auto x : tiles){
        x.update(pixels);
    }
}
void Thing::randomize(){
    x = (rand() % (stg.map_width - 2)) + 1;
    y = (rand() % (stg.map_height - 2)) + 1;
}
void Resource::update(uint8_t *pixels, Level* lvl){
    if(food)
        for(auto tile : tiles){
            // x = !(bool)(rand() % 50) ? (rand() % stg.map_width) : x;
            // y += 1;
            // randomize();
            tile.x = x;
            tile.y = y;
            tile.update(pixels);
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
Thing::Thing(int x, int y, TileType type):
x(x),
y(y),
type(type){    
}