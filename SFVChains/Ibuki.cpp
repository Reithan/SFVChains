#include "stdafx.h"
#include "Ibuki.h"

using namespace tinyxml2;
Ibuki::Ibuki() : iCharacter() {
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
