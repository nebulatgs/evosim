#include "headers/gene.hpp"

enum class mutationType
{
	pSub,
	pIns,
	pDel,
	gInv,
	gDel
};

std::string Gene::toString()
{
	std::string concat;
	for (auto x : bases)
	{
		char base;
		switch (x)
		{
		case 0:
			base = 'A';
			break;

		case 1:
			base = 'T';
			break;

		case 2:
			base = 'G';
			break;

		case 3:
			base = 'C';
			break;

		default:
			base = 'C';
			break;
		}
		concat.push_back(base);
	}
	return concat;
}