// SFVChains.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Ibuki.h"

int main()
{
	using namespace std;
	Ibuki ninja;

	// Generate Hit Confirms
	set<list<MoveData>> confirm_combos_working, confirm_combos_final;
	for (auto i = ninja._moves.begin(); i != ninja._moves.end(); ++i) {
		if (i->block_adv > -3 &&
			!i->isKnockDown()/* &&
			(i->type & (MoveData::kMVT_Special | MoveData::kMVT_CA | MoveData::kMVT_VT)) == 0*/) {
			list<MoveData> temp;
			temp.push_back(*i);
			confirm_combos_working.insert(temp);
			if ((i->type & MoveData::kMVT_TargetCombo) != 0 &&
				!i->isWhiffable() &&
				(i->hit_adv > 2 || i->cancels != NULL))
				confirm_combos_final.insert(temp);
		}
	}
	bool found = true;
	while (found && !confirm_combos_working.empty()) {
		found = false;
		for (auto i = confirm_combos_working.begin(); i != confirm_combos_working.end();){
			for (auto j = ninja._moves.begin(); j != ninja._moves.end(); ++j) {
				if (/*(j->type & (MoveData::kMVT_VT | MoveData::kMVT_Special | MoveData::kMVT_CA)) != 0 ||*/
					((j->type & MoveData::kMVT_Air) != 0 && !i->back().canCancel(MoveData::kMVT_Air)) ||
					j->isWhiffable())
					continue;
				if ((i->back().block_adv >= j->startup - 1 || i->back().canCancel(*j)) &&
					!j->isKnockDown()) {
					found = true;
					list<MoveData> temp = *i;
					temp.push_back(*j);
					if ((temp.size() > 2 || ((*j).type & MoveData::kMVT_TargetCombo) != 0) && (*j).block_adv > -3 && ((*j).hit_adv > 2 || (*j).cancels != NULL)) {
						confirm_combos_final.insert(temp);
					}
					if (temp.size() < 4)
						confirm_combos_working.insert(temp);
				}
			}
			i = confirm_combos_working.erase(i);
		}
	}
	// Generate Basic Combos
	set<list<MoveData>> basic_combos_working, basic_combos_final;
	//for (auto i = ninja._moves.begin(); i != ninja._moves.end(); ++i) {
	//	if (i->hit_adv > 2 &&
	//		!i->isKnockDown()) {
	//		list<MoveData> temp;
	//		temp.push_back(*i);
	//		basic_combos_working.insert(temp);
	//	}
	//}
	//found = true;
	//while (found && !basic_combos_working.empty()) {
	//	found = false;
	//	for (auto i = basic_combos_working.begin(); i != basic_combos_working.end();) {
	//		bool broke = false;
	//		for (auto j = ninja._moves.begin(); j != ninja._moves.end(); ++j) {
	//			if ((i->back().isKnockDown() && i->back().knockDownAdv() >= j->startup - 1) ||
	//				(!i->back().isKnockDown() && i->back().hit_adv >= j->startup - 1) ||
	//				i->back().canCancel(*j) ||
	//				(i->size() > 1 && i->back().type == MoveData::kMVT_VT && (++(i->rbegin()))->vtc_block_adv >= j->startup - 1)) {
	//				found = true;
	//				basic_combos_working.insert(*i);
	//				list<MoveData> temp = *i;
	//				temp.push_back(*j);
	//				basic_combos_working.insert(temp).first;
	//				int dmg_sum = 0, stun_sum = 0;
	//				float scaling = 1.0f;
	//				bool juggle = false;
	//				for (auto k = i->begin(); k != i->end(); ++k) {
	//					dmg_sum += short(k->damage * scaling);
	//					stun_sum += k->stun;
	//					scaling -= 0.1f;
	//					juggle |= k->isKnockDown();
	//				}
	//				if (i->size() > 2 && (dmg_sum > 250 || i->size() > 4 || i->back().isKnockDown() || i->back().isReset() || stun_sum > 900 /*probable stun*/))
	//					basic_combos_final.insert(*i);
	//				if (i->back().isReset() ||
	//					i->size() > 8 || // 10% scaling
	//					stun_sum > 1000 || // gauranteed stun
	//					dmg_sum > 1000 // fatal
	//					) { 
	//					i = basic_combos_working.erase(i);
	//					broke = true;
	//					break;
	//				}
	//			}
	//		}
	//		if (!broke)
	//			++i;
	//		else
	//			i = basic_combos_working.erase(i);
	//	}
	//}
	// Generate VTC Combos

	// Generate Combos into CA

	// Generate Confirmed Combos

	// Print Results
	cout << "==Hit Confirm Strings < 5 Moves==\n";
	int count = 0;
	for (auto i = confirm_combos_final.begin(); i != confirm_combos_final.end(); ++i) {
		MoveData last_move;
		for (auto j = i->begin(); j != i->end(); ++j) {
			if (j != i->begin()) {
				if (last_move.canCancel(*j))
					cout << " xx ";
				else if (last_move.block_adv < j->startup)
					cout << ", (FT), ";
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
	}
	/*cout << "\n\n==Basic Combos==\n";
	for (auto i = basic_combos_final.begin(); i != basic_combos_final.end(); ++i) {
		int last_adv = 100, damage = 0, stun = 0;
		float scaling = 1.0f;
		for (auto j = i->begin(); j != i->end(); ++j) {
			if (j != i->begin())
				cout << ", ";
			if (last_adv < j->startup)
				cout << "(FT), ";
			cout << j->name;
			last_adv = (j->isKnockDown()) ? j->knockDownAdv() : j->hit_adv;
			damage += short(j->damage * scaling);
			stun += j->stun;
			scaling -= 0.1f;
		}
		if (damage > 1000)
			cout << " - Fatal";
		else if (damage > 900)
			cout << " - Possible Fatal";
		if (stun > 1000)
			cout << " - Stun";
		else if (stun > 900)
			cout << " - Possible Stun";
		cout << endl;
	}*/
	system("pause");
    return 0;
}
