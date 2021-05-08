#pragma once

#include <string>
#include <vector>

enum class mutationType;

struct Gene
{
	std::vector<uint8_t> bases;
	std::string toString();
};