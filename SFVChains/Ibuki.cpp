#include "stdafx.h"
#include "Ibuki.h"

Ibuki::Ibuki() : iCharacter() {
	// Move all this out to an XML or CSV file - TODO
	_moves.push_back(MoveData("stand LP", 3, 8, 2, 3, 3, 4, 30, 70, MoveData::kMVT_Basic, MoveData::kMVT_Basic | MoveData::kMVT_Special | MoveData::kMVT_VT));
	_moves.push_back(MoveData("stand MP", 5, 10, 2, 4, 6, 8, 60, 100, MoveData::kMVT_Basic, MoveData::kMVT_Special | MoveData::kMVT_VT));
	_moves.push_back(MoveData("stand HP", 12, 19, -5, 0, 4, 9, 80, 150, MoveData::kMVT_Basic, MoveData::kMVT_Special | MoveData::kMVT_VT));
	_moves.push_back(MoveData("stand LK", 4, 9, -2, 0, 1, 3, 30, 70, MoveData::kMVT_Basic, MoveData::kMVT_Special | MoveData::kMVT_VT));
	_moves.push_back(MoveData("stand MK", 5, 15, -1, 0, 7, 8, 60, 100, MoveData::kMVT_Basic, MoveData::kMVT_VT));
	_moves.push_back(MoveData("stand HK", 10, 16, -2, 1, 8, 11, 90, 150, MoveData::kMVT_Basic, MoveData::kMVT_VT));
	_moves.push_back(MoveData("crouch LP", 4, 8, 1, 3, 1, 3, 30, 70, MoveData::kMVT_Basic, MoveData::kMVT_Basic | MoveData::kMVT_Special | MoveData::kMVT_VT));
	_moves.push_back(MoveData("crouch MP", 6, 12, -1, 3, 5, 9, 60, 100, MoveData::kMVT_Basic, MoveData::kMVT_Special | MoveData::kMVT_VT));
	_moves.push_back(MoveData("crouch HP", 9, 16, -3, 2, 8, 13, 90, 150, MoveData::kMVT_Basic, MoveData::kMVT_Special | MoveData::kMVT_VT));
	_moves.push_back(MoveData("crouch LK", 4, 8, 1, 3, 2, 4, 20, 70, MoveData::kMVT_Basic, MoveData::kMVT_VT));
	_moves.push_back(MoveData("crouch MK", 6, 16, -3, 0, 3, 6, 60, 100, MoveData::kMVT_Basic, MoveData::kMVT_VT));
	_moves.push_back(MoveData("crouch HK", 8, 24, -12, 67, 5, 84, 100, 150, MoveData::kMVT_Basic | MoveData::kMVT_KnockDown | MoveData::kMVT_HardKnockDown, MoveData::kMVT_VT));
	_moves.push_back(MoveData("6MK", 28, 16, -2, 3, 0, 0, 70, 100, MoveData::kMVT_Basic, NULL));
	_moves.push_back(MoveData("6HK", 14, 16, -3, 2, 0, 0, 90, 150, MoveData::kMVT_Basic, NULL));
	_moves.push_back(MoveData("3MK", 7, 14, -10, -6, 0, 0, 50, 100, MoveData::kMVT_Basic, NULL));
	_moves.push_back(MoveData("4MP", 7, 11, -1, 4, 4, 9, 40, 70, MoveData::kMVT_Basic, MoveData::kMVT_Special | MoveData::kMVT_VT));
	_moves.push_back(MoveData("4MP 2", 11, 9, 0, 4, 3, 7, 30, 50, MoveData::kMVT_Basic, MoveData::kMVT_Special | MoveData::kMVT_VT));
	_moves.push_back(MoveData("TC 5LP 5MP", 3, 12, -8, -5, 1, 4, 40, 50, MoveData::kMVT_TargetCombo | MoveData::kMVT_Basic, MoveData::kMVT_VT));
	_moves.push_back(MoveData("TC 5LP 5MP 5HP", 3, 19, -5, 0, 5, 10, 50, 70, MoveData::kMVT_TargetCombo | MoveData::kMVT_Basic, MoveData::kMVT_VT | MoveData::kMVT_Special));
	_moves.push_back(MoveData("TC 5LP 5MP 5LK", 3, 9, -2, 1, 6, 9, 30, 50, MoveData::kMVT_TargetCombo | MoveData::kMVT_Basic, MoveData::kMVT_VT));
	_moves.push_back(MoveData("TC 5MK 2HK", 5, 24, -12, 68, 0, 80, 60, 60, MoveData::kMVT_Basic | MoveData::kMVT_KnockDown | MoveData::kMVT_HardKnockDown, MoveData::kMVT_VT));
	_moves.push_back(MoveData("TC 5MK 2HK 5HK", 5, 16, 0, 17, 0, 28, 40, 60, MoveData::kMVT_Whiff | MoveData::kMVT_Reset | MoveData::kMVT_KnockDown | MoveData::kMVT_TargetCombo | MoveData::kMVT_Basic, MoveData::kMVT_VT));
	_moves.push_back(MoveData("TC 2MP 6HK", 6, 16, -5, 0, 0, 0, 70, 100, MoveData::kMVT_TargetCombo | MoveData::kMVT_Basic, NULL));
	_moves.push_back(MoveData("TC 5MP 6MK", 5, 16, -2, 5, 0, 0, 70, 100, MoveData::kMVT_TargetCombo | MoveData::kMVT_Basic, NULL));
	_moves.push_back(MoveData("jump LP", 3, 0, 3, 6, 0, 0, 40, 70, MoveData::kMVT_Air, NULL));
	_moves.push_back(MoveData("jump MP", 5, 0, 7, 10, 0, 0, 60, 100, MoveData::kMVT_Air, NULL));
	_moves.push_back(MoveData("jump HP", 9, 0, 9, 12, 0, 0, 90, 150, MoveData::kMVT_Air, NULL));
	_moves.push_back(MoveData("jump LK", 3, 0, 2, 5, 0, 0, 40, 70, MoveData::kMVT_Air, NULL));
	_moves.push_back(MoveData("jump MK", 6, 0, 5, 8, 0, 0, 60, 100, MoveData::kMVT_Air, NULL));
	_moves.push_back(MoveData("jump HK", 8, 0, 8, 11, 0, 0, 90, 150, MoveData::kMVT_Air, NULL));
	_moves.push_back(MoveData("TC 8LP 8MK", 0, 3, 3, 6, 0, 0, 70, 50, MoveData::kMVT_TargetCombo | MoveData::kMVT_Air, NULL));
	_moves.push_back(MoveData("TC 8HP 8HK", 0, 9, 9, 12, 0, 0, 70, 50, MoveData::kMVT_TargetCombo | MoveData::kMVT_Air, NULL));
	_moves.push_back(MoveData("THROW", 5, 18, 64, 64, 0, 0, 120, 170, MoveData::kMVT_KnockDown | MoveData::kMVT_Throw, NULL));
	_moves.push_back(MoveData("THROW BACK", 5, 18, 78, 78, 0, 0, 130, 170, MoveData::kMVT_KnockDown | MoveData::kMVT_Throw, NULL));
	_moves.push_back(MoveData("THROW AIR", 5, 0, 110, 110, 0, 0, 140, 150, MoveData::kMVT_KnockDown | MoveData::kMVT_Air | MoveData::kMVT_AirThrow, NULL));
	_moves.push_back(MoveData("VT", 1, 5, 0, 0, 0, 0, 100, 100, MoveData::kMVT_VT, NULL));
	_moves.push_back(MoveData("VS", 15, 20, -5, 85, 0, 0, 60, 100, MoveData::kMVT_VS | MoveData::kMVT_KnockDown | MoveData::kMVT_KnockBack, NULL));
	_moves.push_back(MoveData("VS HOLD", 25, 18, -2, 87, 0, 0, 80, 150, MoveData::kMVT_VS | MoveData::kMVT_KnockDown | MoveData::kMVT_KnockBack, NULL));
	_moves.push_back(MoveData("VR", 15, 25, -2, 88, 0, 0, 60, 0, MoveData::kMVT_KnockDown | MoveData::kMVT_VR, NULL));
	_moves.push_back(MoveData("Forward Dash", 3, 4, -17, -17, 0, 0, 0, 0, MoveData::kMVT_Dash, NULL));					 // Startup/recover on dashes is guessed based on Kasumigake
	_moves.push_back(MoveData("Back Dash", 3, 4, -21, -21, 0, 0, 0, 0, MoveData::kMVT_Air | MoveData::kMVT_Dash, NULL)); // Startup/recover on dashes is guessed based on Kasumigake
	_moves.push_back(MoveData("Nobusuma", 21, 31, -32, -32, 0, 0, 0, 0, MoveData::kMVT_Air | MoveData::kMVT_Dash, NULL));
	_moves.push_back(MoveData("Kasumigake LK", 3, 4, -21, -21, 0, 0, 0, 0, MoveData::kMVT_Special | MoveData::kMVT_Dash, NULL));
	_moves.push_back(MoveData("Kasumigake MK", 3, 1, -23, -23, 0, 0, 0, 0, MoveData::kMVT_Special | MoveData::kMVT_Dash, NULL));
	_moves.push_back(MoveData("Kasumigake HK", 7, 4, -57, -57, 0, 0, 0, 0, MoveData::kMVT_Special | MoveData::kMVT_Dash | MoveData::kMVT_Jump, MoveData::kMVT_Air | MoveData::kMVT_AirThrow));
	_moves.push_back(MoveData("Kazekiri LK", 3, 34, -27, 81, 0, 0, 120, 150, MoveData::kMVT_Special | MoveData::kMVT_KnockDown | MoveData::kMVT_KnockBack, MoveData::kMVT_CA));
	_moves.push_back(MoveData("Kazekiri MK", 4, 34, -27, 81, 0, 0, 120, 150, MoveData::kMVT_Special | MoveData::kMVT_KnockDown | MoveData::kMVT_KnockBack, MoveData::kMVT_CA));
	_moves.push_back(MoveData("Kazekiri HK", 5, 34, -27, 81, 0, 0, 120, 150, MoveData::kMVT_Special | MoveData::kMVT_KnockDown | MoveData::kMVT_KnockBack, MoveData::kMVT_CA));
	_moves.push_back(MoveData("Kazekiri EX", 4, 37, -27, 85, 0, 0, 120, 150, MoveData::kMVT_Special | MoveData::kMVT_Jump | MoveData::kMVT_KnockDown | MoveData::kMVT_EX | MoveData::kMVT_KnockBack, MoveData::kMVT_CA | MoveData::kMVT_Air));
	_moves.push_back(MoveData("Raida LP", 7, 30, -12, 79, 0, 0, 100, 200, MoveData::kMVT_Special | MoveData::kMVT_KnockDown | MoveData::kMVT_KnockBack, NULL));
	_moves.push_back(MoveData("Raida MP", 7, 30, -12, 77, 0, 0, 110, 200, MoveData::kMVT_Special | MoveData::kMVT_KnockDown | MoveData::kMVT_KnockBack, NULL));
	_moves.push_back(MoveData("Raida HP", 7, 30, -12, 79, 0, 0, 120, 200, MoveData::kMVT_Special | MoveData::kMVT_KnockDown | MoveData::kMVT_KnockBack, NULL));
	_moves.push_back(MoveData("Raida EX", 7, 27, -11, 79, 0, 0, 150, 200, MoveData::kMVT_Special | MoveData::kMVT_KnockDown | MoveData::kMVT_EX | MoveData::kMVT_KnockBack, NULL));
	_moves.push_back(MoveData("Kunai P", 7, 26, -2, 2, 8, 11, 40, 50, MoveData::kMVT_Special | MoveData::kMVT_Projectile, MoveData::kMVT_CA));
	_moves.push_back(MoveData("Kunai EX", 7, 36, 18, 110, 18, 110, 60, 100, MoveData::kMVT_Special | MoveData::kMVT_KnockDown | MoveData::kMVT_EX | MoveData::kMVT_KnockBack | MoveData::kMVT_Projectile, NULL));
	_moves.push_back(MoveData("Airborne Kunai P", 7, 0, 13, 17, 0, 0, 40, 50, MoveData::kMVT_Special | MoveData::kMVT_Air | MoveData::kMVT_Projectile, NULL));
	_moves.push_back(MoveData("Airborne Kunai EX", 7, 0, 24, 22, 0, 0, 70, 100, MoveData::kMVT_Special | MoveData::kMVT_KnockDown | MoveData::kMVT_Air | MoveData::kMVT_KnockBack | MoveData::kMVT_Projectile, NULL));
	_moves.push_back(MoveData("Kunai Ikkinage", 11, 26, 9, 110, 0, 0, 150, 150, MoveData::kMVT_KnockDown | MoveData::kMVT_Special | MoveData::kMVT_Projectile, MoveData::kMVT_CA));
	_moves.push_back(MoveData("Airborne Kunai Ikkinage", 16, 0, 0, 0, 0, 0, 150, 150, MoveData::kMVT_Special | MoveData::kMVT_KnockDown | MoveData::kMVT_Air | MoveData::kMVT_Projectile, NULL));
	_moves.push_back(MoveData("Kunai Hoju K 1", 11, 43, -54, -54, 0, 0, 0, 0, MoveData::kMVT_Special, NULL));
	_moves.push_back(MoveData("Kunai Hoju K 2", 24, 43, -74, -74, 0, 0, 0, 0, MoveData::kMVT_Special, NULL));
	_moves.push_back(MoveData("Kunai Hoju K 3", 36, 43, -82, -82, 0, 0, 0, 0, MoveData::kMVT_Special, NULL));
	_moves.push_back(MoveData("Kunai Hoju K 4", 48, 43, -99, -99, 0, 0, 0, 0, MoveData::kMVT_Special, NULL));
	_moves.push_back(MoveData("Kunai Hoju K 5", 60, 43, -103, -103, 0, 0, 0, 0, MoveData::kMVT_Special, NULL));
	_moves.push_back(MoveData("Kunai Hoju K 6", 72, 43, -115, -115, 0, 0, 0, 0, MoveData::kMVT_Special, NULL));
	_moves.push_back(MoveData("Critical Art - Kachofugetsu", 5, 17, 20, 110, 0, 0, 350, 0, MoveData::kMVT_CA | MoveData::kMVT_KnockDown | MoveData::kMVT_HardKnockDown | MoveData::kMVT_KnockBack, NULL));
	// missing nobusumu air float
}

/*virtual*/ bool Ibuki::isValidCombo(const Combo& combo) const {
	int kunai = 6;
	for (auto i = combo.begin(); i != combo.end(); ++i) {
		// check kunai inventory
		if ((*i)->name.find("Ikkinage") != (*i)->name.npos)
			kunai = 0;
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
