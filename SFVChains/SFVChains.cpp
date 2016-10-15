// SFVChains.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Ibuki.h"

using namespace std;
void clearConsole(bool clear = false) {
	HANDLE hOut;
	COORD Position;
	DWORD Written;

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	Position.X = 0;
	Position.Y = 0;
	if(clear)
		FillConsoleOutputCharacter(hOut, ' ', 1000000, Position, &Written);

	SetConsoleCursorPosition(hOut, Position);
}

void pauseConsole() {
	cout << "\nPress enter to continue.";
	cin.clear();
	cin.sync();
	cin.ignore();
}

void GenerateHitConfirms(iCharacter::ComboList& confirm_combos_final, iCharacter* fighter){
	clearConsole(true);
	iCharacter::ComboList confirm_combos_working;
	for (auto i = fighter->_moves.begin(); i != fighter->_moves.end(); ++i) {
		if (i->blockAdv() > -3 &&
				!i->isKnockDown()) {
			iCharacter::Combo temp;
			temp.push_back(&*i);
			confirm_combos_working.push(temp);
			if (!i->isWhiffable() &&
					i->hasType(MoveData::kMVT_TargetCombo) &&
					(i->hitAdv(MoveData::kHAT_Raw) > 2 || i->canCancel()) &&
					!i->hasType(MoveData::kMVT_Throw | MoveData::kMVT_AirThrow) &&
					fighter->isValidCombo(temp))
				confirm_combos_final.push(temp);
		}
	}
	for (auto* i = &confirm_combos_working.front(); !confirm_combos_working.empty();){
		for (auto j = fighter->_moves.begin(); j != fighter->_moves.end(); ++j) {
			if (((j->hasType(MoveData::kMVT_Air)) == 0 || i->back()->canCancelInto(MoveData::kMVT_Air)) &&
					((i->back()->hasType(MoveData::kMVT_KnockBack)) == 0 || j->hasType(MoveData::kMVT_Dash)) &&
					!j->isWhiffable() &&
					j->notType(MoveData::kMVT_VT | MoveData::kMVT_VR | MoveData::kMVT_CA) &&
					(i->back()->blockAdv() >= j->startup - MoveData::kMDC_FrameTrapGap) &&
					!j->isKnockDown() &&
					!j->hasType(MoveData::kMVT_Throw | MoveData::kMVT_AirThrow)) {
				iCharacter::Combo temp = *i;
				temp.push_back(&*j);
				if(fighter->isValidCombo(temp)){
					int push = 0, EX = 3;
					for (auto k = temp.begin(); EX >= 0 && k != temp.end(); ++k) {
						if ((*k)->hasType(MoveData::kMVT_EX))
							--EX;
						if ((*k)->hasType(MoveData::kMVT_TargetCombo)) {
							string::size_type start = -1;
							while ((start = (*k)->name.find("L", ++start)) != (*k)->name.npos)
								push += 2;
							start = 0;
							while ((start = (*k)->name.find("M", ++start)) != (*k)->name.npos)
								push += 3;
							start = 0;
							while ((start = (*k)->name.find("H", ++start)) != (*k)->name.npos)
								push = 0;
						}
						else if ((*k)->hasType(MoveData::kMVT_Basic)) {
							if ((*k)->name.find("L") != (*k)->name.npos)
								push += 2;
							if ((*k)->name.find("M") != (*k)->name.npos)
								push += 3;
							if ((*k)->name.find("H") != (*k)->name.npos)
								push -= 1;
						}
						else
							push = 0;
						if ((*k)->hasType(MoveData::kMVT_Basic) && push > 5)
							break;
					}
					if (push > 5)
						continue;
					if (temp.size() < 4)
						confirm_combos_working.push(temp);
					else if ((*j).blockAdv() > -3 && ((*j).hitAdv(MoveData::kHAT_Raw) > 2 || (*j).canCancel())) {
						confirm_combos_final.push(temp);
					}
				}
			}
		}
		int push = 0;
		for (auto k = i->begin(); k != i->end(); ++k) {
			if ((*k)->hasType(MoveData::kMVT_TargetCombo)) {
				string::size_type start = -1;
				while ((start = (*k)->name.find("L", ++start)) != (*k)->name.npos)
					push += 2;
				start = 0;
				while ((start = (*k)->name.find("M", ++start)) != (*k)->name.npos)
					push += 3;
				start = 0;
				while ((start = (*k)->name.find("H", ++start)) != (*k)->name.npos)
					push = 0;
			}
			else if ((*k)->hasType(MoveData::kMVT_Basic)) {
				if ((*k)->name.find("L") != (*k)->name.npos)
					push += 2;
				if ((*k)->name.find("M") != (*k)->name.npos)
					push += 3;
				if ((*k)->name.find("H") != (*k)->name.npos)
					push -= 1;
			}
			else
				push = 0;
			if ((*k)->hasType(MoveData::kMVT_Basic) && push > 5)
				break;
		}
		if (push < 6 && fighter->isValidCombo(*i) && (i->size() > 2 || i->back()->hasType(MoveData::kMVT_TargetCombo)) && i->back()->blockAdv() > -3 && (i->back()->hitAdv(MoveData::kHAT_Raw) > 2 || i->back()->canCancel()))
			confirm_combos_final.push(*i);
		confirm_combos_working.pop();
		if (!confirm_combos_working.empty())
			i = &confirm_combos_working.front();
		clearConsole();
		cout << "==Confirm Combos Processing==\nSeed combos in queue:\t" << confirm_combos_working.size() << "\nFinal combos:\t" << confirm_combos_final.size() << '\n';;
	}
}

void GenerateBasicCombos(iCharacter::ComboList& basic_combos_final, iCharacter* fighter){
	clearConsole(true);
	iCharacter::ComboList basic_combos_working;
	for (auto i = fighter->_moves.begin(); i != fighter->_moves.end(); ++i) {
		if ((i->hitAdv() > 2 &&
				!i->isKnockDown()) ||
				i->canCancel()) {
			iCharacter::Combo temp;
			temp.push_back(&*i);
			if(fighter->isValidCombo(temp))
				basic_combos_working.push(temp);
		}
	}
	for (auto* i = &basic_combos_working.front(); !basic_combos_working.empty();){
		bool added = false;
		for (auto j = fighter->_moves.begin(); j != fighter->_moves.end(); ++j) {
			if (!i->back()->isKnockDown(true) && !i->back()->hasType(MoveData::kMVT_Throw | MoveData::kMVT_AirThrow) &&
				(j->notType(MoveData::kMVT_AirThrow) || (j->hasType(MoveData::kMVT_AirThrow) && i->back()->hasType(MoveData::kMVT_KnockDown | MoveData::kMVT_KnockBack | MoveData::kMVT_Reset))) &&
				(j->notType(MoveData::kMVT_Air) || i->back()->canCancelInto(MoveData::kMVT_Air)) &&
					(i->back()->notType(MoveData::kMVT_KnockBack) || (i->back()->hasType(MoveData::kMVT_KnockBack) && j->hasType(MoveData::kMVT_Dash | MoveData::kMVT_Projectile))) &&
					(i->back()->notType(MoveData::kMVT_Jump) || (i->back()->hasType(MoveData::kMVT_Jump) && j->hasType(MoveData::kMVT_Air))) &&
					j->notType(MoveData::kMVT_VT | MoveData::kMVT_VR | MoveData::kMVT_CA) &&
					(/*i->back()->canCancelInto(*j) || */
					((j->notType(MoveData::kMVT_Dash) || (j->hasType(MoveData::kMVT_Dash) && i->back()->hitAdv() + j->hitAdv() + j->startup > 2)) &&
						((i->back()->notType(MoveData::kMVT_Dash) && i->back()->hitAdv() >= j->startup) ||
							(i->size() > 1 &&  i->back()->hasType(MoveData::kMVT_Dash) && i->back()->hitAdv() + (++*(i->rbegin()))->hitAdv() >= j->startup)))) &&
					(!(i->back()->isReset() || i->back()->isKnockDown()) || j->notType(MoveData::kMVT_Throw))) {
				iCharacter::Combo temp = *i;
				temp.push_back(&*j);
				if(fighter->isValidCombo(temp)){
					int damage = 0, stun = 0, push = 0, EX = 3;
					float scaling = 1.0f;
					for(auto k = temp.begin(); EX >= 0 && k != temp.end(); ++k){
						damage += int((*k)->damage * scaling);
						scaling -= 0.1f;
						stun += (*k)->stun;
						if((*k)->hasType(MoveData::kMVT_EX))
							--EX;
						if((*k)->hasType(MoveData::kMVT_TargetCombo)){
							string::size_type start = -1;
							while((start = (*k)->name.find("L",++start)) != (*k)->name.npos)
								push += 2;
							start = 0;
							while((start = (*k)->name.find("M",++start)) != (*k)->name.npos)
								push += 3;
							start = 0;
							while((start = (*k)->name.find("H",++start)) != (*k)->name.npos)
								push = 0;
						}
						else if((*k)->hasType(MoveData::kMVT_Basic)){
							if((*k)->name.find("L") != (*k)->name.npos)
								push += 2;
							if((*k)->name.find("M") != (*k)->name.npos)
								push += 3;
							if((*k)->name.find("H") != (*k)->name.npos)
								push -= 1;
						}
						else
							push = 0;
						if ((*k)->hasType(MoveData::kMVT_Basic) && push > 5)
							break;
					}
					if(push > 5 || EX < 0)
						continue;
					if (scaling > 0.0f && damage < 250 && stun < 900 && 
						(temp.size() > 1 && (j->notType(MoveData::kMVT_Dash) || 
							(j->hasType(MoveData::kMVT_Dash) && j->hitAdv() + i->back()->hitAdv() > 2)))){
						basic_combos_working.push(temp);
						added = true;
					}
					else if ((damage >= 250 || stun >= 900 || j->isKnockDown() ||
							(i->back()->isKnockDown() && j->isReset())) &&
							(
								(j->notType(MoveData::kMVT_Dash) ||
								(temp.size() > 1 && j->hasType(MoveData::kMVT_Dash) &&
									(j->hitAdv() + i->back()->hitAdv() > -3))))){
						basic_combos_final.push(temp);
					}
				}
			}
		}
		if(fighter->isValidCombo(*i)){
			int damage = 0, stun = 0, push = 0;
			for(auto k = i->begin(); k != i->end(); ++k){
				damage += (*k)->damage;
				stun += (*k)->stun;
				if((*k)->hasType(MoveData::kMVT_TargetCombo)){
					string::size_type start = -1;
					while((start = (*k)->name.find("L",++start)) != (*k)->name.npos)
						push += 2;
					start = 0;
					while((start = (*k)->name.find("M",++start)) != (*k)->name.npos)
						push += 3;
					start = 0;
					while((start = (*k)->name.find("H",++start)) != (*k)->name.npos)
						push = 0;
				}
				else if((*k)->hasType(MoveData::kMVT_Basic)){
					if((*k)->name.find("L") != (*k)->name.npos)
						push += 2;
					if((*k)->name.find("M") != (*k)->name.npos)
						push += 3;
					if((*k)->name.find("H") != (*k)->name.npos)
						push -= 1;
				}
				else
					push = 0;
				if ((*k)->hasType(MoveData::kMVT_Basic) && push > 5)
					break;
			}
			if (push < 6 && i->size() > 1 && !added && ((damage >= 250 || stun >= 900 || i->back()->isKnockDown() ||
							((++*(i->rbegin()))->isKnockDown() && i->back()->isReset())) &&
							(
								(i->back()->notType(MoveData::kMVT_Dash) ||
								(i->back()->hasType(MoveData::kMVT_Dash) &&
									(i->back()->hitAdv() + (++*(i->rbegin()))->hitAdv() > -3))))))
				basic_combos_final.push(*i);
		}
		basic_combos_working.pop();
		if (!basic_combos_working.empty())
			i = &basic_combos_working.front();
		clearConsole();
		cout << "==Basic Combos Processing==\nSeed combos in queue:\t" << basic_combos_working.size() << "\nFinal combos:\t" << basic_combos_final.size() << '\n';
	}
}

void outputComboList(iCharacter::ComboList combos, ostream& output = cout) {
	int count = 0;
	for (auto* i = &combos.front(); !combos.empty();) {
		const MoveData* last_move;
		int damage = 0, stun = 0;
		float scaling = 1.0f;
		for (auto j = i->begin(); j != i->end(); ++j) {
			if (j != i->begin()) {
				if (last_move->canCancelInto(**j))
					output << " xx ";
				else if (last_move->hitAdv() < (*j)->startup)
					output << " [FT] ";
				else
					output << ", ";
			}
			output << (*j)->name;
			last_move = *j;
			damage += short((*j)->damage * scaling);
			stun += (*j)->stun;
			scaling -= 0.1f;
		}
		if (damage > 1000)
			output << " - Fatal";
		else {
			if (damage > 900)
				output << " - Possible Fatal";
			if (stun > 1000)
				output << " - Stun";
			else if (stun > 900)
				output << " - Possible Stun";
			if (last_move->isReset() && (++*(i->rbegin()))->isKnockDown())
				output << " - Vortex";
			else if (last_move->hasType(MoveData::kMVT_HardKnockDown))
				output << " - Hard Knockdown";
			else if (last_move->isKnockDown())
				output << " - Knockdown";
		}
		output << " - " << damage << " Damage, " << stun << " Stun\n";
		if (&output == &cout && ++count % 50 == 0) {
			pauseConsole();
			clearConsole(true);
		}
		combos.pop();
		if (!combos.empty())
			i = &combos.front();
	}
	if (&output == &cout)
		pauseConsole();
}

int main()
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = false;
	lpCursor.dwSize = 20; //default size
	SetConsoleCursorInfo(console, &lpCursor);

	Ibuki ninja;
	fstream file;

	// Generate Hit Confirms
	iCharacter::ComboList confirm_combos;
	GenerateHitConfirms(confirm_combos, &ninja);
	file.open("Hit Confirms.txt", ios::out | ios::trunc);
	if (file.is_open()) {
		outputComboList(confirm_combos, file);
		file.close();
	}
	pauseConsole();
	clearConsole(true);

	// Generate Basic Combos
	iCharacter::ComboList basic_combos;
	GenerateBasicCombos(basic_combos, &ninja);
	file.open("Basic Combos.txt", ios::out | ios::trunc);
	if (file.is_open()) {
		outputComboList(basic_combos, file);
		file.close();
	}
	pauseConsole();
	clearConsole(true);

	// Generate VTC Combos

	// Generate Combos into CA

	// Generate Confirmed Combos

	// Print Results
	clearConsole(true);
	cout << "==Hit Confirm Strings < 5 Moves==\n";
	outputComboList(confirm_combos);
	clearConsole(true);
	cout << "\n\n==Basic Combos==\n";
	outputComboList(basic_combos);
	return 0;
}
