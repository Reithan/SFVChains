#include "stdafx.h"
#include "Ibuki.h"

using namespace tinyxml2;
Ibuki::Ibuki() : iCharacter() {
	XMLDocument move_file;
	move_file.LoadFile("Resources/Ibuki.xml");
	auto move_list = move_file.RootElement();
	if(!move_list->Attribute("Name","Ibuki"))
		return;
	for (auto i = move_list->FirstChildElement(); i != nullptr; i = i->NextSiblingElement()) {
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
		for (unsigned int j = 1; (1 << (j-1)) <= MoveData::kMVT_MaxFlag; ++j) {
			if(nullptr != types_node->FirstChildElement(MoveData::kFlagNames[j].c_str()))
				types |= (1 << (j-1));
			if(nullptr != cancels_node->FirstChildElement(MoveData::kFlagNames[j].c_str()))
				cancels |= (1 << (j-1));
		}
		_moves.push_back(MoveData(name, startup, recovery, block_adv, hit_adv, damage, stun, types, cancels));
	}
}

/*virtual*/ bool Ibuki::isValidCombo(const Combo& combo) const {
	int kunai = 6;
	for (auto i = combo.begin(); i != combo.end(); ++i) {
		// check kunai inventory
		if ((*i)->name.find("Ikkinage") != (*i)->name.npos) {
			if (kunai > 0)
				kunai = 0;
			else
				return false;
		}
		else if ((*i)->name.find("Hoju") != (*i)->name.npos)
			kunai = MIN(6, kunai + (*i)->name.back() - '0');
		else if ((*i)->name.find("Kunai") != (*i)->name.npos)
			--kunai;
		// check VT stock - TODO
		// check Bomb TiMINg stock - TODO
		// check Dash Spacing stock
		if (i != combo.begin()) {
			const std::string& name = (*std::prev(i))->name;
			if (name == "Raida EX" || name == "Kunai EX" || name == "Kazekiri EX") {
				if ((*i)->name == "Kasumigake LK")
					return false;
				if ((*i)->name == "Kasumigake HK" && name == "Kunai EX")
					return false;
			}
			else if (name.find("Raida") != name.npos) {
				if ((*i)->name == "Kasumigake LK")
					return false;
			}
			else if (name.find("Kazekiri") != name.npos) {
				if ((*i)->name == "Kasumigake HK")
					return false;
			}
		}
	}
	return kunai >= 0;
};
