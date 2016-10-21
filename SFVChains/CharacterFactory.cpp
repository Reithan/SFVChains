#include "stdafx.h"
#include "CharacterFactory.h"
#include "Ibuki.h"

using namespace std;
namespace CharacterFactory {
template <class C>
unique_ptr<iCharacter> CreateCharacter() { return std::unique_ptr<iCharacter>(new C); }
unordered_map<string, unique_ptr<iCharacter>(*)()> constructor_map = {
	{"Ibuki", CreateCharacter<Ibuki>}
};
unique_ptr<iCharacter> LoadCharacter(const string& char_name, const string& char_file) {
	auto iter = constructor_map.find(char_name);
	if (constructor_map.end() != iter) {
		auto new_char = iter->second();
		if (new_char->Initialize(char_name, char_file))
			return new_char;
	}
	return nullptr;
}
}