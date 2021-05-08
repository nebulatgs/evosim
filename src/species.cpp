#include "headers/species.hpp"
#include "headers/creature.hpp"

std::vector<Gene> Species::getDefaultGenome()
{
	return {
		{{2, 0, 0}}, // Set vision gene
		{{11, 11, 11}},
		{{0, 0, 0}},

		{{0, 1, 0}},
		{{0, 2, 0}},
		{{0, 3, 0}},

		{{1, 0, 1}}, // Read antibiotic resistance gene
		{{5, 5, 5}},
		{{1, 0, 2}},

		{{1, 0, 3}}, // Read size gene
		{{5, 5, 5}},
		{{1, 1, 0}}};
}

Species::Species()
{
	genome = getDefaultGenome();
}
Species::Species(Creature *initial_creature)
{
	genome = initial_creature->genome;
}