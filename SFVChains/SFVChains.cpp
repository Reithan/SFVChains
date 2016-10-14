// SFVChains.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Ibuki.h"

using namespace std;

void GenerateHitConfirms(set<iCharacter::Combo>& confirm_combos_final, iCharacter* fighter){
	set<iCharacter::Combo> confirm_combos_working;
	for (auto i = fighter->_moves.begin(); i != fighter->_moves.end(); ++i) {
		if (i->blockAdv() > -3 &&
				!i->isKnockDown()) {
			iCharacter::Combo temp;
			temp.push_back(*i);
			confirm_combos_working.insert(temp);
			if (!i->isWhiffable() &&
					i->hasType(MoveData::kMVT_TargetCombo) &&
					(i->hitAdv(MoveData::kHAT_Raw) > 2 || i->canCancel()) &&
					!i->hasType(MoveData::kMVT_Throw | MoveData::kMVT_AirThrow) &&
					fighter->isValidCombo(temp))
				confirm_combos_final.insert(temp);
		}
	}
	bool found = true;
	while (found && !confirm_combos_working.empty()) {
		found = false;
		for (auto i = confirm_combos_working.begin(); i != confirm_combos_working.end();){
			for (auto j = fighter->_moves.begin(); j != fighter->_moves.end(); ++j) {
				if (((j->hasType(MoveData::kMVT_Air)) == 0 || i->back().canCancelInto(MoveData::kMVT_Air)) &&
						((j->hasType(MoveData::kMVT_KnockBack)) == 0 || i->back().hasType(MoveData::kMVT_Dash)) &&
						!j->isWhiffable() &&
						(i->back().blockAdv() >= j->startup - MoveData::kMDC_FrameTrapGap || i->back().canCancelInto(*j)) &&
						!j->isKnockDown() &&
						!j->hasType(MoveData::kMVT_Throw | MoveData::kMVT_AirThrow)) {
					found = true;
					iCharacter::Combo temp = *i;
					temp.push_back(*j);
					if(fighter->isValidCombo(temp)){
						if (temp.size() < 4)
							confirm_combos_working.insert(temp);
						else if ((*j).blockAdv() > -3 && ((*j).hitAdv(MoveData::kHAT_Raw) > 2 || (*j).canCancel())) {
							confirm_combos_final.insert(temp);
						}
					}
				}
			}
			if (fighter->isValidCombo(*i) && (i->size() > 2 || i->back().hasType(MoveData::kMVT_TargetCombo)) && i->back().blockAdv() > -3 && (i->back().hitAdv(MoveData::kHAT_Raw) > 2 || i->back().canCancel()))
				confirm_combos_final.insert(*i);
			i = confirm_combos_working.erase(i);
		}
	}
}

void GenerateBasicCombos(set<iCharacter::Combo>& basic_combos_final, iCharacter* fighter){
	set<iCharacter::Combo> basic_combos_working;
	for (auto i = fighter->_moves.begin(); i != fighter->_moves.end(); ++i) {
		if (i->blockAdv() > -3 &&
				!i->isKnockDown()) {
			iCharacter::Combo temp;
			temp.push_back(*i);
			if(fighter->isValidCombo(temp))
				basic_combos_working.insert(temp);
		}
	}
	bool found = true;
	while (found && !basic_combos_working.empty()) {
		found = false;
		for (auto i = basic_combos_working.begin(); i != basic_combos_working.end();){
			bool added = false;
			for (auto j = fighter->_moves.begin(); j != fighter->_moves.end(); ++j) {
				if ((j->notType(MoveData::kMVT_Air) || i->back().canCancelInto(MoveData::kMVT_Air)) &&
						(j->notType(MoveData::kMVT_KnockBack) || i->back().hasType(MoveData::kMVT_Dash)) &&
						j->notType(MoveData::kMVT_VT) &&
						((!i->back().hasType(MoveData::kMVT_Dash) && i->back().hitAdv() >= j->startup) ||
							(i->back().hasType(MoveData::kMVT_Dash) && i->back().hitAdv() + (++(i->rbegin()))->hitAdv() >= j->startup) ||
							i->back().canCancelInto(*j)) &&
						(!(i->back().isReset() || i->back().isKnockDown()) || j->notType(MoveData::kMVT_Throw))) {
					found = true;
					iCharacter::Combo temp = *i;
					temp.push_back(*j);
					if(fighter->isValidCombo(temp)){
						int damage = 0, stun = 0, push = 0;
						for(auto k = temp.begin(); k != temp.end(); ++k){
							damage += k->damage;
							stun += k->stun;
							if(k->hasType(MoveData::kMVT_TargetCombo)){
								string::size_type start = -1;
								while((start = k->name.find("L",++start)) != k->name.npos)
									push += 2;
								start = 0;
								while((start = k->name.find("M",++start)) != k->name.npos)
									push += 3;
								start = 0;
								while((start = k->name.find("H",++start)) != k->name.npos)
									push += 4;
							}
							else if(k->hasType(MoveData::kMVT_Basic)){
								if(k->name.find("L") != k->name.npos)
									push += 2;
								if(k->name.find("M") != k->name.npos)
									push += 3;
								if(k->name.find("H") != k->name.npos)
									push += 4;
							}
							else
								push = 0;
						}
						if (push < 9 && damage < 250 && stun < 900 && (temp.size() > 1 && (j->notType(MoveData::kMVT_Dash) || i->back().canCancelInto(MoveData::kMVT_Dash) && j->hitAdv() > 2) || (j->hasType(MoveData::kMVT_Dash) && j->hitAdv() + i->back().hitAdv() > 2))){
							basic_combos_working.insert(temp);
							added = true;
						}
						else if (push < 9 && (damage >= 250 || stun >= 900 || j->isKnockDown() || (temp.size() > 1 && i->back().isKnockDown() && j->isReset())) && ((j->notType(MoveData::kMVT_Dash) || i->back().canCancelInto(MoveData::kMVT_Dash) && j->hitAdv() > -3) || (j->hasType(MoveData::kMVT_Dash) && j->hitAdv() + i->back().hitAdv() > -3))){
							basic_combos_final.insert(temp);
						}
					}
				}
			}
			if(fighter->isValidCombo(*i)){
				int damage = 0, stun = 0, push = 0;
				for(auto k = i->begin(); k != i->end(); ++k){
					damage += k->damage;
	        stun += k->stun;
					if(k->hasType(MoveData::kMVT_TargetCombo)){
						string::size_type start = -1;
						while((start = k->name.find("L",++start)) != k->name.npos)
							push += 2;
						start = 0;
						while((start = k->name.find("M",++start)) != k->name.npos)
							push += 3;
						start = 0;
						while((start = k->name.find("H",++start)) != k->name.npos)
							push += 4;
					}
					else if(k->hasType(MoveData::kMVT_Basic)){
						if(k->name.find("L") != k->name.npos)
							push += 2;
						if(k->name.find("M") != k->name.npos)
							push += 3;
						if(k->name.find("H") != k->name.npos)
							push += 4;
					}
					else
						push = 0;
				}
				if (!added && (push < 9 && (damage >= 250 || stun >= 900 || i->back().isKnockDown() || (i->size() > 1 && (++(i->rbegin()))->isKnockDown() && i->back().isReset())) && ((i->back().notType(MoveData::kMVT_Dash) || (++(i->rbegin()))->canCancelInto(MoveData::kMVT_Dash) && i->back().hitAdv() > -3) || (i->back().hasType(MoveData::kMVT_Dash) && i->back().hitAdv() + (++(i->rbegin()))->hitAdv() > -3))))
					basic_combos_final.insert(*i);
			}
			i = basic_combos_working.erase(i);
		}
	}
}

int main()
{
	Ibuki ninja;

	// Generate Hit Confirms
	set<iCharacter::Combo> confirm_combos;
	GenerateHitConfirms(confirm_combos, &ninja);

	// Generate Basic Combos
	set<iCharacter::Combo> basic_combos;
	GenerateBasicCombos(basic_combos, &ninja);

	// Generate VTC Combos

	// Generate Combos into CA

	// Generate Confirmed Combos

	// Print Results
	cout << "==Hit Confirm Strings < 5 Moves==\n";
	int count = 0;
	/*for (auto i = confirm_combos_final.begin(); i != confirm_combos_final.end(); ++i) {
		MoveData last_move;
		for (auto j = i->begin(); j != i->end(); ++j) {
			if (j != i->begin()) {
				if (last_move.canCancelInto(*j))
					cout << " xx ";
				else if (last_move.blockAdv() < j->startup)
					cout << " [FT] ";
				else
					cout << ", ";
			}
			cout << j->name;
			last_move = *j;
		}
		cout << endl;
		if (++count % 50 == 0) {
			system("pause");
			system("cls");
		}
	}*/
	cout << "\n\n==Basic Combos==\n";
	for (auto i = basic_combos.begin(); i != basic_combos.end(); ++i) {
		MoveData last_move;
		int damage = 0, stun = 0;
		float scaling = 1.0f;
		for (auto j = i->begin(); j != i->end(); ++j) {
			if (j != i->begin()) {
				if (last_move.canCancelInto(*j))
					cout << " xx ";
				else if (last_move.hitAdv() < j->startup)
					cout << " [FT] ";
				else
					cout << ", ";
			}
			cout << j->name;
			last_move = *j;
			damage += short(j->damage * scaling);
			stun += j->stun;
			scaling -= 0.1f;
		}
		if (damage > 1000)
			cout << " - Fatal";
		else {
			if (damage > 900)
				cout << " - Possible Fatal";
			if (stun > 1000)
				cout << " - Stun";
			else if (stun > 900)
				cout << " - Possible Stun";
			if(last_move.isReset() && (++(i->rbegin()))->isKnockDown())
				cout << " - Vortex";
			else if(last_move.hasType(MoveData::kMVT_HardKnockDown))
				cout << " - Hard Knockdown";
			else if(last_move.isKnockDown())
				cout << " - Knockdown";
		}
		cout << endl;
		if (++count % 50 == 0) {
			system("pause");
			system("cls");
		}
	}
	system("pause");
	return 0;
}
