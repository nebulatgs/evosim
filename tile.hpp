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


// Represents a single tile on the world grid
class Tile{
    public:
        Tile(int x, int y, uint32_t color);
        virtual void update(uint8_t *pixels);
        // virtual void update(std::vector<Tile*> *tiles);
        int x, y;
        // int width, height;
        TileType type;
        uint32_t color;
}; 

class Thing{
    public:
        Thing(int x, int y);
        int x;
        int y;
        uint32_t color;
        std::vector<Tile> tiles;
        virtual void update(uint8_t *pixels);
};

class Creature : public Thing{
    private:
        void pSub();
        void pIns();
        int digitConcat(std::vector<int> digits);

        int transcribeIndex;
        std::vector<int> transcribedProteins;

    protected:
        uint32_t geneIndex;

    public:
        std::vector<Gene> genome;
        int mutate(int count, mutationType mutation);
        Creature(int x, int y, int species);
        void update(uint8_t *pixels);
        int readNext();
        void reset();
        int species;
        uint32_t size;        
};




class Resource : public Thing{
    public:
        Resource(int x, int y, bool food);
        // void draw(uint8_t *pixels);
        void update(uint8_t *pixels);
        bool food;
        uint32_t color = 0xFF6E9055;
};

class Border : public Thing{
    public:
        Border(int x, int y);
        void update(uint8_t *pixels);
        uint32_t color = 0xFF926E5F;
};