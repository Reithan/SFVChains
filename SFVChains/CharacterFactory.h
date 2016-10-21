#pragma once
#include "iCharacter.h"

namespace CharacterFactory {
extern std::unordered_map<std::string, std::unique_ptr<iCharacter>(*)()> constructor_map;
template <class C>
std::unique_ptr<iCharacter> CreateCharacter() { return std::unique_ptr<iCharacter>(new C); }
std::unique_ptr<iCharacter> LoadCharacter(const std::string& char_name, const std::string& char_file);
}
