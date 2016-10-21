#include "stdafx.h"
#include "CharacterFactory.h"

using namespace std;
namespace CharacterFactory {
/*extern*/ unordered_map<string, unique_ptr<iCharacter>(*)()> CharacterFactory::constructor_map;
unique_ptr<iCharacter> LoadCharacter(const string& char_name, const string& char_file) {
	auto iter = constructor_map.find(char_name);
	if (constructor_map.end() == iter)
		iter = constructor_map.begin();
	if(constructor_map.end() == iter)
		return nullptr;
	auto new_char = iter->second();
	if (new_char->Initialize(char_name, char_file))
		return new_char;
	return nullptr;
}
}