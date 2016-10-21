#pragma once
#include "MoveData.h"
#define ADD_TO_MAP(name) struct name; static struct name ## _pre_main_class \
	{ name ## _pre_main_class(){ \
			CharacterFactory::constructor_map[#name] = CharacterFactory::CreateCharacter<name>; \
		} \
	} name ## _pre_main;
struct iCharacter {
	static const unsigned short kPossibleStun = 900;
	static const unsigned short kConfirmedStun = 1050;
	static const unsigned short kPossibleFatal = 900;
	static const unsigned short kConfirmedFatal = 1050;
	concurrency::concurrent_vector<MoveData> _moves;
	iCharacter() {}
	virtual ~iCharacter() {}
	bool Initialize(const std::string& character_name, const std::string& filename);
	virtual bool isValidCombo(const Combo& combo) const = 0;
};
