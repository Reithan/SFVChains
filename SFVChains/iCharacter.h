#pragma once
#include "MoveData.h"

struct iCharacter {
	concurrency::concurrent_vector<MoveData> _moves;
	iCharacter() {}
	virtual ~iCharacter() {}
	virtual bool isValidCombo(const Combo& combo) const = 0;
};