#include "tile.hpp"
#include <iostream>
#include "proteins.hpp"
// extern int tilesX, tilesY;
enum class mutationType {pSub, pIns, pDel, gInv, gDel};
enum class TileType {Wall, Resource, Creature};
extern std::vector<std::vector<Gene>> ecosystem;

class Level{
    public:
        // Level();
        std::vector<Thing*> things;
        void update(uint8_t *pixels);
        long oldTime;
        bool antibiotic;
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
    genome = getDefaults();
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
bool Creature::findFood(int radius, void* buffer){
    // std::cout << radius << '\n';
    int index = 0;
    v2d distance = v2d(99999,99999);
    int food = -1;
    for(auto thing : current_lvl->things){
        if(thing->type == TileType::Resource){
            if (pos.x == thing->pos.x && pos.y == thing->pos.y){                    
                return 0;
            }                
            v2d dist = v2d(abs(thing->pos.x - pos.x),abs(thing->pos.y - pos.y));                
            v2d vision = v2d(radius, radius);
            if(dist < distance && dist < vision && ((Resource*)thing)->food){
                distance = dist;
                food = index;
            }                
        }
        index++;
    }
    // *(Thing**)buffer = search[index];
    *(int*)buffer = food;
    return 0;
}

bool Creature::moveWander(){
    float fac = 0.5 * stg.sleep;
    if(wanderTime != time(nullptr)) {
        wanderDirection = v2d(1,1);
        float r = ((float)rand()/(float)RAND_MAX);
        wanderDirection = (wanderDirection.rotate(r*M_PI*2));
        wanderDirection.setLen(fac);
        // tempX = currentDirection->x, tempY = currentDirection->y;
        wanderTime = time(nullptr);
    }
    wanderDirection.setLen(fac);
    float tempX = wanderDirection.x, tempY = wanderDirection.y;
    pos.x +=  (tempX + pos.x) < stg.map_width - 1 && (tempX + pos.x) > 1 ? tempX : 1;
    pos.y +=  (tempY + pos.y)< stg.map_height - 1 && (tempY + pos.y)> 1 ? tempY : 1;
    energy -= 2 * stg.sleep; 
    return 0;
}

bool Creature::moveFood(){
    v2d dist = closest_food->pos - pos;
    dist.setLen(0.5 * stg.sleep);    
    float tempX = dist.x, tempY = dist.y;
    pos.x +=  (tempX + pos.x) < stg.map_width - 1 && (tempX + pos.x) > 1 ? tempX : 1;
    
    pos.y +=  (tempY + pos.y)< stg.map_height - 1 && (tempY + pos.y)> 1 ? tempY : 1;
    energy -= 2 * stg.sleep; 
    return 0;
}
bool Creature::reproduce(){
    if(energy > 500){
        energy -= 250;
        // mutate();
        Creature* child = new Creature(pos.x, pos.y+1, species);
        child->genome = genome;  
        child->mutate();    
        uint8_t* childColor = (uint8_t*)(&child->color);        
        childColor[1] = (rand() % 256);                
        child->found_food = 0; 
        current_lvl->things.push_back(child);
    }
    return 0;
}
uint32_t oldCol = 0x3C4CE7;
bool Creature::SetARes(int strength){
    a_res = strength;
    uint8_t *startCol = (uint8_t*)&oldCol;
    uint8_t r = (startCol[0] * (100 - strength) + (0x34 * strength)) / 100;
    uint8_t g = (startCol[1] * (100 - strength) + (0x98 * strength)) / 100;
    uint8_t b = (startCol[2] * (100 - strength) + (0xdb * strength)) / 100;
    startCol = (uint8_t*)&color;
    startCol[0] = r;
    startCol[1] = g;
    startCol[2] = b;
    // std::cout << strength << '\n';
    // #3498db
    // color = 0xb98029;
    return 0;
}
void Creature::mutate(){
    // if(((float)rand() / (float)RAND_MAX) * 1000 * (1/stg.sleep) > (1000 * (1/stg.sleep) - 1)){
    //     int i = (int)((float)rand() / (float)RAND_MAX) * genome.size();
    //     int j = (int)((float)rand() / (float)RAND_MAX) * genome[i].bases.size();
    //     if(proteinLUT[genome[i].toString()] < Protein::instr_sep)
    //         genome[i].bases[j] = (int)((float)rand() / (float)RAND_MAX) * 33;
    //     else
    //         genome[i].bases[j] = (int)((float)rand() / (float)RAND_MAX) * 3;
    // // }
    genome[7].bases[rand() % 3] = rand() % 33;
}
bool Creature::eatFood(){
    if(abs(pos.x - closest_food->pos.x) <= 1 && abs(pos.y - closest_food->pos.y) <= 1 && closest_food->food){
        closest_food->randomize();        
        found_food = false;
        energy += 250;
    }
    if(closest_food->pos != foodPos){
        found_food = false;        
    }
    return 0;
}
int Creature::processInstruction(int protein, int memory){
    int result;
    bool error;
    switch(protein){
        case FindFood:
            error = findFood(memory, &result);
            closest_food = (Resource*)current_lvl->things[result];
            break;
        case MoveFood:
            if(memory > -1)
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
        default:
            error = 1;
    }
    if(result == -1)
        return -1;
    return error;
}
std::vector<Gene> Creature::getDefaults(){
    return {
        {{2,0,0}},
        {{9,9,9}},
        {{0, 0, 0}},
        {{0, 1, 0}},
        {{0, 2, 0}},
        {{0, 3, 0}},
        {{1,0,1}}, //Read antibiotic resistance gene
        {{5,5,5}},
        {{1, 0, 2}}
    };
}
bool Creature::processGenome(){
    using namespace std;
    stack<int> brain;
    for(int i = 0; i < genome.size(); i++){
        int protein = proteinLUT[genome[i].toString()];
        // void* instruction;
         bool (*instuction)(int, void*);
        if(protein > Protein::instr_sep){
        // Process instruction
            if(brain.empty()){
                int top = ltbrain.top();
                ltbrain.pop();
                ltbrain.push(processInstruction(protein, top));
            }
            else{
                int top = brain.top();
                brain.pop();
                ltbrain.push(processInstruction(protein, top));
            }
        }
        else{
        // Process value
            i++;
            brain.push(accumulate(genome[i].bases.begin(), genome[i].bases.end(), 0));
        }
    }
    return 0;
}
bool Creature::update(uint8_t *pixels, Level* lvl){            
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
    if(processGenome()){
        return 1;
    }
    for(auto tile : tiles){                                                
            tile.x = pos.x;
            tile.y = pos.y;
            tile.update(pixels, color);
    }
    if(current_lvl->antibiotic){
        // if(((float)rand() / (float)RAND_MAX) * a_res < 1){
        if(!((rand() % 100) < a_res)){
            return 1;
        }
    }
    // Am I dead?
    // if(closest_food == nullptr && !wander)
    //     return 1;
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
        x.update(pixels, color);
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
            tile.update(pixels, color);
        }
        return 0;
    }
    else{
        return 1;
    }
}

void Tile::update(uint8_t *pixels, uint32_t color){
    int index = x * 3 + (y * 3 * stg.map_width);
    if(index > stg.map_height * stg.map_width * 3)
        return;
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