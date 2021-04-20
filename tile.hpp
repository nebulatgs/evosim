#include <vector>
#include <string>
#include <stdint.h>
#include <stack>
#include "v2d.h"
#pragma once

enum class mutationType;
enum class TileType;
// class v2d;
class SDL_Renderer;
struct Gene{
    std::vector<uint8_t> bases;
    std::string toString();
};
class Level;

// Represents a single tile on the world grid
class Tile{
    public:
        Tile(int x, int y, uint32_t color);
        virtual void update(uint8_t *pixels, uint32_t color);
        // virtual void update(std::vector<Tile*> *tiles);
        int x, y;
        // int width, height;
        TileType type;
        uint32_t color;
}; 

class Thing{
    public:
        Thing(int x, int y, TileType type);
        ~Thing();
        void randomize();
        // int x;
        // int y;
        v2d pos;
        uint32_t color;
        TileType type;
        std::vector<Tile> tiles;
        virtual bool update(uint8_t *pixels, Level* lvl);
};

class Resource : public Thing{
    public:
        Resource(int x, int y, bool food);
        // void draw(uint8_t *pixels);
        bool update(uint8_t *pixels, Level* lvl);
        // void randomize();
        bool food;
        uint32_t color = 0xFF6E9055;
};

class Creature : public Thing{
    private:
        void pSub();
        void pIns();
        int digitConcat(std::vector<int> digits);

        int transcribeIndex;
        std::vector<int> transcribedProteins;
        // int distance = RAND_MAX;
        bool wander;
        v2d wanderDirection;
        long wanderTime;
        Resource *closest_food = nullptr;
        bool found_food = false;
        v2d distance = v2d(999999, 999999);
        v2d foodPos = v2d(0, 0);
        Level *current_lvl = nullptr;
        std::vector<Gene> getDefaults();
        int a_res = 0;
        std::stack<int> ltbrain;

    protected:
        bool processGenome();
        int processInstruction(int protein, int memory);
        bool findFood(int radius, void* buffer);
        bool moveFood();
        bool eatFood();
        bool reproduce();
        bool moveWander();
        bool SetARes(int strength);
        void mutate();
        uint32_t geneIndex;

    public:
        std::vector<Gene> genome;
        // std::vector<uint8_t> genome;
        int mutate(int count, mutationType mutation);
        Creature(int x, int y, int species);
        ~Creature();
        bool update(uint8_t *pixels, Level* lvl);
        int readNext();
        void reset();
        int species;
        uint32_t size;
        // uint32_t color = 0x3C4CE7;
        uint32_t color = 0xa879fd;
        float energy = 400;

};





class Border : public Thing{
    public:
        Border(int x, int y);
        bool update(uint8_t *pixels, Level* lvl);
        uint32_t color = 0xFF926E5F;
};