#include "stdafx.h"
#include "iCharacter.h"
using namespace std;
using namespace tinyxml2;
bool iCharacter::Initialize(const string& character_name, const string& filename) {
	XMLDocument move_file;
	move_file.LoadFile(filename.c_str());
	auto move_list = move_file.RootElement();
	if (nullptr == move_list || !move_list->Attribute("Name", character_name.c_str()))
		return false;
	for (auto i = move_list->FirstChildElement("Move"); i != nullptr; i = i->NextSiblingElement("Move")) {
		auto name = i->Attribute("Name");
		auto startup = i->IntAttribute("Startup");
		auto recovery = i->IntAttribute("Recovery");
		auto block_adv = i->IntAttribute("BlockAdvantage");
		auto hit_adv = i->IntAttribute("HitAdvantage");
		auto damage = i->IntAttribute("Damage");
		auto stun = i->IntAttribute("Stun");
		unsigned int types = 0, cancels = 0;
		auto types_node = i->FirstChildElement("Types");
		auto cancels_node = i->FirstChildElement("Cancels");
		for (unsigned int j = 1; (1 << (j - 1)) <= MoveData::kMVT_MaxFlag; ++j) {
			if (nullptr != types_node->FirstChildElement(MoveData::kFlagNames[j].c_str()))
				types |= (1 << (j - 1));
			if (nullptr != cancels_node->FirstChildElement(MoveData::kFlagNames[j].c_str()))
				cancels |= (1 << (j - 1));
		}
		_moves.push_back(MoveData(name, startup, recovery, block_adv, hit_adv, damage, stun, types, cancels));
	}
	return _moves.size() > 0;
}
