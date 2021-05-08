#pragma once

#include "gene.hpp"

class Creature;

class Species
{
public:
	Species();
	Species(Creature* initial_creature);

	float getDifference(Creature* creature);
	std::vector<Gene> getDefaultGenome();
public:
	std::vector<Gene> genome;
};