#pragma once
#include "MoveData.h"

struct iCharacter {
	typedef std::list<const MoveData*> Combo;
	std::vector<MoveData> _moves;
	iCharacter() {}
	virtual ~iCharacter() {}
	virtual bool isValidCombo(const Combo& combo) const = 0;
};