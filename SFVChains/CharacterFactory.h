#pragma once
#include "Ibuki.h"

namespace CharacterFactory {
std::unique_ptr<iCharacter> LoadCharacter(const std::string& char_name, const std::string& char_file);
}
