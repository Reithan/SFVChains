// SFVChains.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Ibuki.h"

#define WHOLE_CONSOLE 10000
using namespace std;
void clearConsole(unsigned int clear = 0) {
	HANDLE hOut;
	COORD Position;
	DWORD Written;

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	Position.X = 0;
	Position.Y = 0;
	if(clear)
		FillConsoleOutputCharacter(hOut, ' ', clear, Position, &Written);

	SetConsoleCursorPosition(hOut, Position);
}

template <typename T>
void printLoc(SHORT X, SHORT Y, const T& buffer, ostream& output = cout) {
	HANDLE hOut;
	COORD Position;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Position.X = X;
	Position.Y = Y;
	SetConsoleCursorPosition(hOut, Position);
	output << buffer;
}

void pauseConsole() {
	cout << "\nPress enter to continue.";
	cin.clear();
	cin.sync();
	cin.ignore();
}

void GenerateHitConfirms(ComboList& confirm_combos_final, iCharacter* fighter){
	clearConsole(WHOLE_CONSOLE);
	cout << "==Confirm Combos Processing==\nSeed combos in queue:\nFinal combos:";
	ComboList confirm_combos_working;
	for (auto i = fighter->_moves.begin(); i != fighter->_moves.end(); ++i) {
		if (i->blockAdv() > -3 &&
				!i->isKnockDown()) {
			Combo temp;
			temp.push_back(&*i);
			confirm_combos_working.push(temp);
			if (!i->isWhiffable() &&
					i->hasAnyType(MoveData::kMVT_TargetCombo) &&
					(i->hitAdv(MoveData::kHAT_Raw) > 2 || i->canCancel()) &&
					!i->hasAnyType(MoveData::kMVT_Throw | MoveData::kMVT_AirThrow) &&
					fighter->isValidCombo(temp))
				confirm_combos_final.push(temp);
		}
	}
	for (auto* i = &confirm_combos_working.front(); !confirm_combos_working.empty();){
		for (auto j = fighter->_moves.begin(); j != fighter->_moves.end(); ++j) {
			if (((j->hasAnyType(MoveData::kMVT_Air)) == 0 || i->back()->canCancelInto(MoveData::kMVT_Air)) && // only allow air follow up if it's cancelled into
					!j->isWhiffable() && // don't hit confirm with whiffables
					j->notAllTypes(MoveData::kMVT_VT | MoveData::kMVT_VR | MoveData::kMVT_CA | MoveData::kMVT_EX) && // dont' waste EX, V, or CA on hit confirms
					((i->back()->canCancelInto(*j) && (i->back()->blockAdv(true) >= j->startup - MoveData::kMDC_FrameTrapGap)) ||  // can cancel into next move
						(i->back()->blockAdv() >= j->startup - MoveData::kMDC_FrameTrapGap)) && // or link / FT into it
					!j->isKnockDown() && // don't hit confirm into KD
					!j->hasAnyType(MoveData::kMVT_Throw | MoveData::kMVT_AirThrow)) { // no throws in hit confirms
				Combo temp = *i;
				temp.push_back(&*j);
				if(fighter->isValidCombo(temp)){
					int damage = 0, stun = 0, push = 0, EX = 3;
					if(!CalculateComboMetrics(temp, damage, stun, push, EX))
						continue;
					if (temp.size() < 4)
						confirm_combos_working.push(temp);
					else if ((*j).blockAdv() > -3 && ((*j).hitAdv(MoveData::kHAT_Raw) > 2 || (*j).canCancel())) {
						confirm_combos_final.push(temp);
					}
				}
			}
		}
		int damage = 0, stun = 0, push = 0, EX = 3;
		if (CalculateComboMetrics(*i, damage, stun, push, EX) && (fighter->isValidCombo(*i) &&
			(i->size() > 2 || i->back()->hasAnyType(MoveData::kMVT_TargetCombo)) && i->back()->blockAdv() > -3 && (i->back()->hitAdv(MoveData::kHAT_Raw) > 2 || i->back()->canCancel())))
			confirm_combos_final.push(*i);
		confirm_combos_working.pop();
		if (!confirm_combos_working.empty())
			i = &confirm_combos_working.front();
		printLoc(25, 1, string(6,' '));
		printLoc(25, 1, confirm_combos_working.size());
		printLoc(25, 2, string(6,' '));
		printLoc(25, 2, confirm_combos_final.size());
	}
}

void GenerateBasicCombos(ComboList& basic_combos_final, iCharacter* fighter){
	clearConsole(WHOLE_CONSOLE);
	cout << "==Basic Combos Processing==\nSeed combos in queue:\nFinal combos:";
	ComboList basic_combos_working;
	for (auto i = fighter->_moves.begin(); i != fighter->_moves.end(); ++i) {
		if ((i->hitAdv() > 2 &&
				!i->isKnockDown()) ||
				i->canCancel()) {
			Combo temp;
			temp.push_back(&*i);
			if(fighter->isValidCombo(temp))
				basic_combos_working.push(temp);
		}
	}
	for (auto* i = &basic_combos_working.front(); !basic_combos_working.empty();){
		bool added = false;
		for (auto j = fighter->_moves.begin(); j != fighter->_moves.end(); ++j) {
			if (!i->back()->isKnockDown(true) && !i->back()->hasAnyType(MoveData::kMVT_Throw | MoveData::kMVT_AirThrow) &&
				(j->notAllTypes(MoveData::kMVT_AirThrow) || (j->hasAnyType(MoveData::kMVT_AirThrow) && i->back()->hasAnyType(MoveData::kMVT_KnockDown | MoveData::kMVT_KnockBack | MoveData::kMVT_Reset))) &&
				(j->notAllTypes(MoveData::kMVT_Air) || i->back()->canCancelInto(MoveData::kMVT_Air)) &&
					(i->back()->notAllTypes(MoveData::kMVT_KnockBack) || (i->back()->hasAnyType(MoveData::kMVT_KnockBack) && j->hasAnyType(MoveData::kMVT_Dash | MoveData::kMVT_Projectile))) &&
					(i->back()->notAllTypes(MoveData::kMVT_Jump) || (i->back()->hasAnyType(MoveData::kMVT_Jump) && j->hasAnyType(MoveData::kMVT_Air))) &&
					j->notAllTypes(MoveData::kMVT_VT | MoveData::kMVT_VR | MoveData::kMVT_CA) &&
					((i->back()->canCancelInto(*j) && i->back()->hitAdv(true) >= j->startup)|| 
					((j->notAllTypes(MoveData::kMVT_Dash) || (j->hasAnyType(MoveData::kMVT_Dash) && i->back()->hitAdv() + j->hitAdv() + j->startup > 2)) &&
						((i->back()->notAllTypes(MoveData::kMVT_Dash) && i->back()->hitAdv() >= j->startup) ||
							(i->size() > 1 &&  i->back()->hasAnyType(MoveData::kMVT_Dash) && i->back()->hitAdv() + (++*(i->rbegin()))->hitAdv() >= j->startup)))) &&
					(!(i->back()->isReset() || i->back()->isKnockDown()) || j->notAllTypes(MoveData::kMVT_Throw))) {
				Combo temp = *i;
				temp.push_back(&*j);
				if(fighter->isValidCombo(temp)){
					int damage = 0, stun = 0, push = 0, EX = 3;
					if (!CalculateComboMetrics(temp, damage, stun, push, EX))
						continue;
					if (temp.size() > 1 && temp.size() < 10 && damage < 250 && stun < 900 &&
						((j->notAllTypes(MoveData::kMVT_Dash) || 
							(j->hasAnyType(MoveData::kMVT_Dash) && j->hitAdv() + i->back()->hitAdv() > 2)))){
						basic_combos_working.push(temp);
						added = true;
					}
					else if (temp.size() < 11 && 
						(damage >= 250 || stun >= 900 || j->isKnockDown() || 
							(i->back()->isKnockDown() && j->isReset())) &&
						( (j->notAllTypes(MoveData::kMVT_Dash) || 
							(temp.size() > 1 && j->hasAnyType(MoveData::kMVT_Dash) &&
								(j->hitAdv() + i->back()->hitAdv() > -3))))){
						basic_combos_final.push(temp);
					}
				}
			}
		}
		if(fighter->isValidCombo(*i)){
			int damage = 0, stun = 0, push = 0, EX = 3;
			if (!CalculateComboMetrics(*i, damage, stun, push, EX) && i->size() > 1 && !added && 
					((damage >= 250 || stun >= 900 || i->back()->isKnockDown() ||
						((++*(i->rbegin()))->isKnockDown() && i->back()->isReset())) &&
						(
							(i->back()->notAllTypes(MoveData::kMVT_Dash) ||
							(i->back()->hasAnyType(MoveData::kMVT_Dash) &&
								(i->back()->hitAdv() + (++*(i->rbegin()))->hitAdv() > -3))))))
				basic_combos_final.push(*i);
		}
		basic_combos_working.pop();
		if (!basic_combos_working.empty())
			i = &basic_combos_working.front();
		printLoc(25, 1, string(6, ' '));
		printLoc(25, 1, basic_combos_working.size());
		printLoc(25, 2, string(6, ' '));
		printLoc(25, 2, basic_combos_final.size());
	}
}

void outputComboList(ComboList combos, const string& header, ostream& output = cout) {
	int count = 0;
	output << header << endl;
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
		if (damage > WHOLE_CONSOLE)
			output << " - Fatal";
		else {
			if (damage > 900)
				output << " - Possible Fatal";
			if (stun > WHOLE_CONSOLE)
				output << " - Stun";
			else if (stun > 900)
				output << " - Possible Stun";
			if (last_move->isReset() && (++*(i->rbegin()))->isKnockDown())
				output << " - Vortex";
			else if (last_move->hasAnyType(MoveData::kMVT_HardKnockDown))
				output << " - Hard Knockdown";
			else if (last_move->isKnockDown())
				output << " - Knockdown";
		}
		output << " - " << damage << " Damage, " << stun << " Stun\n";
		if (&output == &cout && ++count % 50 == 0) {
			pauseConsole();
			clearConsole(WHOLE_CONSOLE);
			output << header << endl;
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
	ComboList confirm_combos;
	GenerateHitConfirms(confirm_combos, &ninja);
	file.open("Hit Confirms.txt", ios::out | ios::trunc);
	if (file.is_open()) {
		outputComboList(confirm_combos, "==Hit Confirm Strings < 5 Moves==", file);
		file.close();
	}
	pauseConsole();
	clearConsole(100);

	// Generate Basic Combos
	ComboList basic_combos;
	GenerateBasicCombos(basic_combos, &ninja);
	file.open("Basic Combos.txt", ios::out | ios::trunc);
	if (file.is_open()) {
		outputComboList(basic_combos, "==Basic Combos==", file);
		file.close();
	}
	pauseConsole();
	clearConsole(100);

	// Generate VTC Combos

	// Generate Combos into CA

	// Generate Confirmed Combos

	// Print Results
	clearConsole(100);
	outputComboList(confirm_combos, "==Hit Confirm Strings < 5 Moves==");
	clearConsole(100);
	outputComboList(basic_combos, "==Basic Combos==");
	return 0;
}
