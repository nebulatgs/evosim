#include <vector>
#include <string>
#include <stdint.h>

#pragma once

enum class mutationType;
enum class TileType;
class SDL_Renderer;

struct Gene{
    std::vector<uint8_t> bases;
    std::string toString();
};


class Organism{
    private:
        void pSub();
        void pIns();
    protected:
        uint32_t geneIndex;
    public:
        std::vector<Gene> genome;
        int mutate(int count, mutationType mutation);
};

class Species : public Organism{
    public:
        int speciesId;
};

// Represents a single tile on the world grid
class Tile{
    public:
        Tile(int x, int y, int width, int height);
        virtual void draw(SDL_Renderer *renderer);
        virtual void update(std::vector<Tile*> *tiles);
        int x, y;
        int width, height;
        TileType type;
        uint32_t color;
}; 

class Creature : public Organism{
    private:
        int digitConcat(std::vector<int> digits);

        int transcribeIndex;
        std::vector<int> transcribedProteins;
        
    public:
        Creature(Species *_species);
        void update(std::vector<Tile*> *tiles);
        int readNext();
        void reset();

        Species *species;
        uint32_t size;        
};



// A type of tile inhabited by a creature
class Cell : public Tile{
    Cell(int x, int y, int width, int height, Creature *creature);
    void draw(SDL_Renderer *renderer);
    void update(std::vector<Tile*> *tiles);
    Creature *creature;
};

class Resource : public Tile{
    public:
        Resource(int x, int y, int width, int height, bool food);
        void draw(SDL_Renderer *renderer);
        void update(std::vector<Tile*> *tiles);
        bool food;
        uint32_t color = 0xFF6E9055;
};

class Border : public Tile{
    public:
        Border(int x, int y, int width, int height);
        void draw(SDL_Renderer *renderer);
        uint32_t color = 0xFF926E5F;
};