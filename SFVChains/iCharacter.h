#pragma once
#include "MoveData.h"

struct iCharacter {
	std::vector<MoveData> _moves;
	iCharacter() {}
	virtual ~iCharacter() {}
};