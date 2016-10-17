// SFVChains.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Ibuki.h"

#define WHOLE_CONSOLE 10000
using namespace std;
using namespace concurrency;
atomic<unsigned int> max_tasks = 0;
void updateMaxTasks(atomic<unsigned int>& new_count) {
	unsigned int current_max_tasks = max_tasks;
	while (new_count > current_max_tasks && !max_tasks.compare_exchange_weak(current_max_tasks, new_count)); // no body
}

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
	atomic<unsigned int> task_count = 0;
	cout << "==Confirm Combos Processing==\nSeed combos in queue:\nFinal combos:";
	ComboList confirm_combos_working;
	parallel_for_each(fighter->_moves.begin(), fighter->_moves.end(),
			[&](auto& i) {
		++task_count;
		updateMaxTasks(task_count);
		if (i.blockAdv() > -3 &&
			!i.isKnockDown()) {
			Combo temp;
			temp.push_back(&i);
			confirm_combos_working.push(temp);
			if (!i.isWhiffable() &&
				i.hasAnyType(MoveData::kMVT_TargetCombo) &&
				(i.hitAdv(MoveData::kHAT_Raw) > 2 || i.canCancel()) &&
				!i.hasAnyType(MoveData::kMVT_Throw | MoveData::kMVT_AirThrow) &&
				fighter->isValidCombo(temp))
				confirm_combos_final.push(temp);
		}
		--task_count;
	});
	task_group group;
	while (task_count > 0 || !confirm_combos_working.empty()) {
		Combo combo;
		if (confirm_combos_working.try_pop(combo)) {
			++task_count;
			updateMaxTasks(task_count);
			group.run([&, combo] {
				parallel_for_each(fighter->_moves.begin(), fighter->_moves.end(), [&] (auto &j){
					if (((j.hasAnyType(MoveData::kMVT_Air)) == 0 || combo.back()->canCancelInto(MoveData::kMVT_Air)) && // only allow air follow up if it's cancelled into
						!j.isWhiffable() && // don't hit confirm with whiffables
						j.notAllTypes(MoveData::kMVT_VT | MoveData::kMVT_VR | MoveData::kMVT_CA | MoveData::kMVT_EX) && // dont' waste EX, V, or CA on hit confirms
						((combo.back()->canCancelInto(j) && (combo.back()->blockAdv(true) >= j.startup - MoveData::kMDC_FrameTrapGap)) ||  // can cancel into next move
						(combo.back()->blockAdv() >= j.startup - MoveData::kMDC_FrameTrapGap)) && // or link / FT into it
						!j.isKnockDown() && // don't hit confirm into KD
						!j.hasAnyType(MoveData::kMVT_Throw | MoveData::kMVT_AirThrow)) { // no throws in hit confirms
						Combo temp = combo;
						temp.push_back(&j);
						if (fighter->isValidCombo(temp)) {
							int damage = 0, stun = 0, push = 0, EX = 3;
							if (CalculateComboMetrics(temp, damage, stun, push, EX)) {
								if (temp.size() < 4)
									confirm_combos_working.push(temp);
								else if ((j).blockAdv() > -3 && ((j).hitAdv(MoveData::kHAT_Raw) > 2 || (j).canCancel())) {
									confirm_combos_final.push(temp);
								}
							}
						}
					}
				});
				int damage = 0, stun = 0, push = 0, EX = 3;
				if (CalculateComboMetrics(combo, damage, stun, push, EX) && (fighter->isValidCombo(combo) &&
					(combo.size() > 2 || combo.back()->hasAnyType(MoveData::kMVT_TargetCombo)) && combo.back()->blockAdv() > -3 && (combo.back()->hitAdv(MoveData::kHAT_Raw) > 2 || combo.back()->canCancel())))
					confirm_combos_final.push(combo);
				--task_count;
			});
		}
		printLoc(25, 1, string(6, ' '));
		printLoc(25, 1, confirm_combos_working.unsafe_size());
		printLoc(25, 2, string(6, ' '));
		printLoc(25, 2, confirm_combos_final.unsafe_size());
	}
	group.wait();
}

void GenerateBasicCombos(ComboList& basic_combos_final, iCharacter* fighter){
	clearConsole(WHOLE_CONSOLE);
	atomic<unsigned int> task_count = 0;
	cout << "==Basic Combos Processing==\nSeed combos in queue:\nFinal combos:";
	ComboList basic_combos_working;
	parallel_for_each(fighter->_moves.begin(), fighter->_moves.end(),
			[&](auto& i) {
		++task_count;
		updateMaxTasks(task_count);
		if ((i.hitAdv() > 2 &&
				!i.isKnockDown()) ||
				i.canCancel()) {
			Combo temp;
			temp.push_back(&i);
			if(fighter->isValidCombo(temp))
				basic_combos_working.push(temp);
		}
		--task_count;
	});
	task_group group;
	while (task_count > 0 || !basic_combos_working.empty()) {
		Combo combo;
		if (basic_combos_working.try_pop(combo)) {
			++task_count;
			updateMaxTasks(task_count);
			group.run([&, combo] {
				bool added = false;
				parallel_for_each(fighter->_moves.begin(), fighter->_moves.end(), [&](auto &j) {
					if (!combo.back()->isKnockDown(true) && combo.back()->notAnyType(MoveData::kMVT_Throw | MoveData::kMVT_AirThrow) && // last move wasn't hard KD or throw
						(j.notAllTypes(MoveData::kMVT_AirThrow) || combo.back()->hasAnyType(MoveData::kMVT_Reset)) && // only air throw out of a reset (unlikely)
						(j.notAllTypes(MoveData::kMVT_Air) || combo.back()->canCancelInto(MoveData::kMVT_Air)) && // only cancel into an air move, can't link air->air
						(combo.back()->notAllTypes(MoveData::kMVT_KnockBack) || j.hasAnyType(MoveData::kMVT_Dash | MoveData::kMVT_Projectile)) && // only go to projectile or dash from knockback
						(combo.back()->notAllTypes(MoveData::kMVT_Jump) || j.hasAnyType(MoveData::kMVT_Air)) && // only go from jump -> air
						j.notAnyType(MoveData::kMVT_VT | MoveData::kMVT_VR | MoveData::kMVT_CA) && // don't include VT, VR, or CA in basic combos
						((combo.back()->canCancelInto(j) && combo.back()->hitAdv(true) >= j.startup) || // cancel if possible
							((j.notAllTypes(MoveData::kMVT_Dash) || combo.back()->hitAdv() + j.hitAdv() + j.startup > 2) && // check if this move is possible if it's a dash
								((combo.back()->notAllTypes(MoveData::kMVT_Dash) && combo.back()->hitAdv() >= j.startup) || // check adv if last move wasn't dash
									(combo.back()->hasAllTypes(MoveData::kMVT_Dash) && // if it was a dash...
									combo.size() > 1 && // ...and there was a move before it...
									combo.back()->hitAdv() + (*(++combo.rbegin()))->hitAdv() >= j.startup))))) { // ...check adv
						Combo temp = combo;
						temp.push_back(&j);
						if (fighter->isValidCombo(temp)) {
							int damage = 0, stun = 0, push = 0, EX = 3;
							if (CalculateComboMetrics(temp, damage, stun, push, EX)) {
								if (temp.size() > 1 && temp.size() < 10 && damage < 250 && stun < 900 &&
									(j.notAllTypes(MoveData::kMVT_Dash) || j.hitAdv() + combo.back()->hitAdv() > 2)) {
									basic_combos_working.push(temp);
									added = true;
								}
								else if ((damage >= 250 || stun >= 900 || j.isKnockDown() ||	(combo.back()->isKnockDown() && j.isReset())) &&
									(j.notAllTypes(MoveData::kMVT_Dash) ||(j.hitAdv() + combo.back()->hitAdv() > -3))) {
									basic_combos_final.push(temp);
								}
							}
						}
					}
				});
				if (fighter->isValidCombo(combo)) {
					int damage = 0, stun = 0, push = 0, EX = 3;
					if (CalculateComboMetrics(combo, damage, stun, push, EX) && combo.size() > 1 && !added &&
						((damage >= 250 || stun >= 900 || combo.back()->isKnockDown() || ((*(++combo.rbegin()))->isKnockDown() && combo.back()->isReset())) &&
						(combo.back()->notAllTypes(MoveData::kMVT_Dash) || (combo.back()->hitAdv() + (*(++combo.rbegin()))->hitAdv() > -3))))
						basic_combos_final.push(combo);
				}
				--task_count;
			});
		}
		printLoc(25, 1, string(6, ' '));
		printLoc(25, 1, basic_combos_working.unsafe_size());
		printLoc(25, 2, string(6, ' '));
		printLoc(25, 2, basic_combos_final.unsafe_size());
	}
	group.wait();
}

void outputComboList(ComboList combos, const string& header, ostream& output = cout) {
	int count = 0;
	output << header << endl;
	for (Combo i;combos.try_pop(i);) {
		const MoveData* last_move;
		int damage = 0, stun = 0;
		float scaling = 1.0f;
		for (auto j = i.begin(); j != i.end(); ++j) {
			if (j != i.begin()) {
				if (last_move->canCancelInto(**j))
					output << " xx ";
				else if (last_move->hitAdv() < (*j)->startup && last_move->notAnyType(MoveData::kMVT_Dash))
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
		output << " - " << damage << " Damage, " << stun << " Stun\n";
		if (damage > 1000)
			output << " - Fatal";
		else {
			if (damage > 900)
				output << " - Possibly Fatal";
			if (stun > 1000)
				output << " - Stun";
			else if (stun > 900)
				output << " - Possibly Stun";
			if (last_move->isReset() && (++*(i.rbegin()))->isKnockDown())
				output << " - Vortex";
			else if (last_move->hasAnyType(MoveData::kMVT_HardKnockDown))
				output << " - Hard Knockdown";
			else if (last_move->isKnockDown())
				output << " - Knockdown";
		}
		if (&output == &cout && ++count % 50 == 0) {
			pauseConsole();
			clearConsole(WHOLE_CONSOLE);
			output << header << endl;
		}
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
	cout << "\nMax tasks queued:\t" << max_tasks << "\nSaving to File...";
	file.open("Hit Confirms.txt", ios::out | ios::trunc);
	if (file.is_open()) {
		outputComboList(confirm_combos, "==Hit Confirm Strings < 5 Moves==", file);
		file.close();
	}
	pauseConsole();
	clearConsole(WHOLE_CONSOLE);
	
	// Generate Basic Combos
	ComboList basic_combos;
	GenerateBasicCombos(basic_combos, &ninja);
	cout << "\nMax tasks queued:\t" << max_tasks << "\nSaving to File...";
	file.open("Basic Combos.txt", ios::out | ios::trunc);
	if (file.is_open()) {
		outputComboList(basic_combos, "==Basic Combos==", file);
		file.close();
	}
	pauseConsole();
	clearConsole(WHOLE_CONSOLE);
	
	// Generate VTC Combos

	// Generate Combos into CA

	// Generate Confirmed Combos

	// Print Results
	clearConsole(WHOLE_CONSOLE);
	outputComboList(confirm_combos, "==Hit Confirm Strings < 5 Moves==");
	clearConsole(WHOLE_CONSOLE);
	outputComboList(basic_combos, "==Basic Combos==");
	return 0;
}
