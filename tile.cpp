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
    // const uint16_t tilesX, tilesY;
    // uint16_t init_width, init_height;
    float scale, drag;
    bool trails;
    float sleep;
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
    tiles.push_back(Tile(pos.x, pos.y, color));
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
bool Creature::update(uint8_t *pixels, Level* lvl){
    // if(randDensity(100)){
    //     // lvl->things.push_back(new Creature(x+1, y, species));
    // }
    if(!found_food){
        closest_food = nullptr;
        distance.set(1000000,1000000);
        int index = 0;
        for(auto thing : lvl->things){
            if(thing->type == TileType::Resource){
                index++;
                if (pos.x == thing->pos.x && pos.y == thing->pos.y){
                    // thing->randomize();
                    return 0;
                }
                // int dist = abs(sqrt((pos.x - thing->pos.x) * (pos.x - thing->pos.x) + (pos.y - thing->pos.y) * (pos.y - thing->pos.y)));
                v2d dist = v2d(abs(thing->pos.x - pos.x),abs(thing->pos.y - pos.y));
                // && (thing->pos.x - pos.x== thing->pos.y -pos.y || thing->pos.x - pos.x ==0 || thing->pos.y -pos.y ==0)
                v2d vision = v2d(30,30);
                if(dist < distance && dist < vision && ((Resource*)thing)->food){
                    distance = dist;
                    closest_food = (Resource*)thing;
                    foodPos = thing->pos;
                }
                // y += thing->y > 0 ? 1 : -1;
            }
        }
        if(closest_food == nullptr && index == 0)
            return 1;
        else if(closest_food == nullptr){
            wander = 1;
        }
        else {
            wander = 0;
        }
        found_food = 1;
    }

        // found_food = closest_food != nullptr;
        // std::cout << closest_food->pos.x << '\n';
    v2d dist = closest_food->pos - pos;
    dist.setLen(0.5 * stg.sleep);
    // int tempX = (closest_food->pos.x - pos.x > 0 ? 1 : -1);
    float tempX = dist.x, tempY = dist.y;
    if(wander) {
        if(wanderTime != time(nullptr)) {
            float fac = 0.5f * stg.sleep;
            // float rand1 = (rand()/(float)RAND_MAX) * 10.0f;
            // float rand2 = (rand()/(float)RAND_MAX) * 10.0f;
            // tempX = (((rand() - ((float)RAND_MAX/rand1)) / (float)RAND_MAX) ) * fac;
            // tempY = (((rand() - ((float)RAND_MAX/rand2)) / (float)RAND_MAX) ) * fac;
            wanderDirection = v2d(1,1);
            float r = ((float)rand()/(float)RAND_MAX);
            wanderDirection = wanderDirection.rotate(r*M_PI*2);
            wanderDirection.setLen(fac);
            tempX = wanderDirection.x, tempY = wanderDirection.y;
            wanderTime = time(nullptr);
        }
        else {
            tempX = wanderDirection.x, tempY = wanderDirection.y;
        }
    }
    pos.x +=  (tempX + pos.x) < stg.map_width && (tempX + pos.x) > 0 ? tempX : 0;

    // int tempY = (closest_food->pos.y - pos.y > 0 ? 1 : -1);
    pos.y +=  (tempY + pos.y)< stg.map_height && (tempY + pos.y)> 0 ? tempY : 0;
    energy -= 2 * stg.sleep;
    // std::cout << stg.sleep << '\n';
    if(energy > 1000){
        energy -= 500;
        Creature* child = new Creature(pos.x, pos.y+1, species);
        // Creature* newCell = (Creature*)(lvl->things.back());
        uint8_t* childColor = (uint8_t*)(&child->color);
        // childColor[0] += rand()% 20 -10;
        childColor[1] = (rand() % 256);
        // childColor[2] += rand()% 20 -10;
        // newCell->closest_food = nullptr;
        child->found_food = 0; 
        lvl->things.push_back(child);
    }
    // int index = (int)pos.x * 3 + ((int)pos.y * 3 * stg.map_width);
    // uint8_t col[3] = {pixels[index++], pixels[index++], pixels[index++]};
    // uint32_t myColor = *((uint32_t*)col);
    // std::cout << myColor << '\n';
    // if((int)pos.x == (int)closest_food->pos.x && (int)pos.y == (int)closest_food->pos.y || myColor == 0xFF6E9055){
    // Resource* food = (Resource*)closest_food;
    if(abs(pos.x - closest_food->pos.x) <= 1 && abs(pos.y - closest_food->pos.y) <= 1){
        closest_food->randomize();
        // closest_food->food = 0;
        found_food = false;
        energy += 250;
    }
    if(closest_food->pos != foodPos){
        found_food = false;
        // closest_food = nullptr;
    }
    for(auto tile : tiles){
            // x = !(bool)(rand() % 50) ? (rand() % stg.map_width) : x;
            // y += 1;
            // randomize();
            // tile.color = 0xEEEEEE;
            tile.x = pos.x;
            tile.y = pos.y;
            tile.update(pixels);
    }
    if(closest_food == nullptr && !wander)
        return 1;
    return !energy;
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

bool Border::update(uint8_t *pixels, Level* lvl){
    for(auto x : tiles){
        x.update(pixels);
    }
    return 0;
}
void Thing::randomize(){
    pos.x = (rand() % (stg.map_width - 2)) + 1;
    pos.y = (rand() % (stg.map_height - 2)) + 1;
}
bool Resource::update(uint8_t *pixels, Level* lvl){
    if(food){
        for(auto tile : tiles){
            // x = !(bool)(rand() % 50) ? (rand() % stg.map_width) : x;
            // y += 1;
            // randomize();
            tile.x = pos.x;
            tile.y = pos.y;
            tile.update(pixels);
        }
        return 0;
    }
    else{
        return 1;
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
// x(x),
// y(y),
type(type){    
    pos.x = x;
    pos.y = y;
}

Thing::~Thing(){
    tiles.empty();
}

Creature::~Creature(){
    tiles.empty();
    transcribedProteins.empty();
    genome.empty();
}