#pragma once
#include "iCharacter.h"
#include "CharacterFactory.h"

ADD_TO_MAP(Ibuki);
struct Ibuki : public iCharacter {
	Ibuki();
	virtual bool isValidCombo(const Combo& combo) const;
};
