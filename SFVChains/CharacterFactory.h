#pragma once
#include "iCharacter.h"

/*
This macro declares, creates and deletes an object
within an anonymous namespace in order to invoke
a constructor that has been repurposed to add the
passed in class name's constructor to the Factory's
constructor lookup map. This should be placed in
each child class's file in order to make it
accessible from the Factory.
*/
#define ADD_TO_MAP(name) \
	namespace { \
	struct name ## _pre_main_struct \
	{ \
		name ## _pre_main_struct() { \
			CharacterFactory::constructor_map[#name] = CharacterFactory::CreateCharacter<name>; \
		} \
	}; \
	void* name ## _pre_main = []()->void*{ delete (new name ## _pre_main_struct()); return nullptr; }(); \
	}

namespace CharacterFactory {
extern std::unordered_map<std::string, std::unique_ptr<iCharacter>(*)()> constructor_map;
template <class C>
inline std::unique_ptr<iCharacter> CreateCharacter() { return std::unique_ptr<iCharacter>(new C); }
std::unique_ptr<iCharacter> LoadCharacter(const std::string& char_name, const std::string& char_file);
}
