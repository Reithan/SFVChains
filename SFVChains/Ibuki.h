#pragma once
#include "iCharacter.h"

struct Ibuki : public iCharacter {
	Ibuki();
	virtual bool isValidCombo(const Combo& combo) const;
};