#include "tile.hpp"
#include <iostream>
extern int tilesX, tilesY;
enum class mutationType {pSub, pIns, pDel, gInv, gDel};
enum class TileType {Air, Food, Creature};

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




void Organism::pSub(){
    int geneIndex = ((float)rand() / (float)RAND_MAX) * genome.size();
    int baseIndex = ((float)rand() / (float)RAND_MAX) * genome[geneIndex].bases.size();
    genome[geneIndex].bases[baseIndex] = rand() % 2;
}
void Organism::pIns(){

}
int Organism::mutate(int count, mutationType mutation = mutationType::pSub){
    switch (mutation){
        case mutationType::pIns:
            pSub(); 
            break;
        
        default:
            pSub();
            break;
    }
    return 0;
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

int transcribeIndex = 0;
std::vector<int> transcribedProteins;

Creature::Creature(Species *_species) : species(_species), size(1){
    genome = _species->genome;
}
int Creature::readNext(){
    // int key = digitConcat({
    //     genome[geneIndex].bases[transcribeIndex],
    //     genome[geneIndex].bases[transcribeIndex+1],
    //     genome[geneIndex].bases[transcribeIndex+2]
    // });
    std::string key = genome[geneIndex].toString();
    std::cout << key << '\n';
    int protein = 0;
    // if(proteinLUT.count(key))
    //     protein = proteinLUT.at(key);
    transcribedProteins.push_back(protein);
    return transcribeIndex+=3;
}
void Creature::reset(){
    transcribedProteins.clear();
}



// Represents a single tile on the world grid


// A type of tile inhabited by a creature
Cell::Cell(int x, int y, int width, int height, Creature *creature):
    Tile(x,y,width,height),
    creature(creature){
}
void Cell::draw(uint8_t *pixels){
    // gTileTexture.render( mBox.x - camera.x, mBox.y - camera.y, &gTileClips[ mType ] );
};

void Cell::update(std::vector<Tile*> *tiles){

}

Resource::Resource(int x, int y, int width, int height, bool food):
    Tile(x,y,width,height),
    food(food){
}

void Resource::draw(uint8_t *pixels){
    // filledCircleColor(renderer, x, y, width/2, 0xFF4DC274);
    // if(food)
        // boxColor(renderer, x, y, x + width, y + height, color);
        
    // aacircleColor(renderer, x, y, width / 2, 0xFF4DC274);
    // int i, j, c;
    // int index = 0;
    // for (i = 0; i < tilesX; i++) {
        // for (j = 0; j < tilesY; j++) {
            uint32_t col = food ? color : 0;
            int index = x * 3 + (y * 3 * 1024);
            pixels[index++] = ((uint8_t*)&col)[0];
            pixels[index++] = ((uint8_t*)&col)[1];
            pixels[index++] = ((uint8_t*)&col)[2];        
        // }
    // }
}

Border::Border(int x, int y, int width, int height):
    Tile(x,y,width,height){
}

void Border::draw(uint8_t *pixels){
    // filledCircleColor(renderer, x, y, width/2, 0xFF4DC274);
    // boxColor(renderer, x, y, x + width, y + height, color);
        
    // aacircleColor(renderer, x, y, width / 2, 0xFF4DC274);
    // int i, j, c;
    // int index = 0;
    // for (i = 0; i < tilesX; i++) {
        // for (j = 0; j < tilesY; j++) {
            int index = x * 3 + (y * 3 * 1024);
            pixels[index++] = ((uint8_t*)&color)[0];
            pixels[index++] = ((uint8_t*)&color)[1];
            pixels[index++] = ((uint8_t*)&color)[2]; 
        // }
    // }
}

void Resource::update(std::vector<Tile*> *tiles){

}

void Tile::draw(uint8_t *pixels){
    
}

void Tile::update(std::vector<Tile*> *tiles){
    
}
Tile::Tile(int x, int y, int width, int height):
    x(x),
    y(y),
    width(width),
    height(height){
}