#include "stdafx.h"
#include "MoveData.h"

using namespace std;
MoveData::MoveData() :
	name(""),
	startup(0),
	recovery(0),
	block_adv(0),
	hit_adv(0),
	vtc_block_adv(0),
	vtc_hit_adv(0),
	damage(0),
	stun(0),
	type((MoveTypes)0),
	cancels((MoveTypes)0) {}

MoveData::MoveData(
		string name,
		short startup,
		short recovery,
		short block_adv,
		short hit_adv,
		short vtc_block_adv,
		short vtc_hit_adv,
		short damage,
		short stun,
		unsigned int type,
		unsigned int cancels
	) :
	name(name),
	startup(startup),
	recovery(recovery),
	block_adv(block_adv),
	hit_adv(hit_adv),
	vtc_block_adv(vtc_block_adv),
	vtc_hit_adv(vtc_hit_adv),
	damage(damage),
	stun(stun),
	type((MoveTypes)type),
	cancels((MoveTypes)cancels)
{}

short MoveData::hitAdv(bool cancel, HitAdvantageType hat_type) const {
	short adv = 0;
	if (hat_type == kHAT_Raw || this->notAnyType(kMVT_KnockDown | kMVT_KnockBack | kMVT_HardKnockDown | kMVT_Throw | kMVT_AirThrow))
		adv = hit_adv;
	else if (hat_type == kHAT_Juggle)
		adv =  hit_adv - kMVT_KnockDownRecover - kMDC_FallDownFrames;
	else if (hat_type == kHAT_Recover)
		adv =  hit_adv - kMVT_KnockDownRecover;
	else if (hat_type == kHAT_RecoverBack)
		adv =  hit_adv - kMVT_KnockDownRecoverBack;
	else
		adv =  hit_adv;
	if (cancel)
		adv -= recovery;
	return adv;
}

short MoveData::hitAdvVTC(HitAdvantageType hat_type) const {
	if (hat_type == kHAT_Raw || this->notAnyType(kMVT_KnockDown | kMVT_KnockBack | kMVT_HardKnockDown | kMVT_Throw | kMVT_AirThrow))
		return vtc_hit_adv;
	else if (hat_type == kHAT_Juggle)
		return vtc_hit_adv - kMVT_KnockDownRecover - kMDC_FallDownFrames;
	else if (hat_type == kHAT_Recover)
		return vtc_hit_adv - kMVT_KnockDownRecover;
	else if (hat_type == kHAT_RecoverBack)
		return vtc_hit_adv - kMVT_KnockDownRecoverBack;
	else
		return vtc_hit_adv;
}

bool CalculateComboMetrics(const Combo& combo, /*out params*/ int& damage, int& stun, int& push, int& EX)
{
	float scaling = 1.0f;
	for (auto k = combo.begin(); EX >= 0 && k != combo.end(); ++k) {
		damage += int((*k)->damage * ((*k)->hasAllTypes(MoveData::kMVT_CA) ? MAX(.5f, scaling) : scaling));
		scaling -= 0.1f;
		stun += (*k)->stun;
		if ((*k)->hasAnyType(MoveData::kMVT_Basic) && push > 3) // most basics can only hit from relatively close
			return false;
		// if not a jump, dash, or projectile, check against larger gap
		if ((*k)->notAnyType(MoveData::kMVT_Air | MoveData::kMVT_Dash | MoveData::kMVT_Projectile) && push > 6)
			return false;
		if ((*k)->hasAnyType(MoveData::kMVT_EX) && --EX < 0) // have we used up all out EX stock?
			return false;
		if ((*k)->hasAnyType(MoveData::kMVT_TargetCombo)) { // grab all move push from TCs
			int tc_push = 0;
			for (string::size_type start = (*k)->name.find("L"); start != (*k)->name.npos; start = (*k)->name.find("L", ++start))
				tc_push += 2;
			for (string::size_type start = (*k)->name.find("M"); start != (*k)->name.npos; start = (*k)->name.find("M", ++start))
				tc_push += 3;
			for (string::size_type start = (*k)->name.find("H"); start != (*k)->name.npos; start = (*k)->name.find("H", ++start))
				tc_push -= 1;
			push += tc_push / 2; // TC's actually push a lot less than linked combos
			if (push > 5) // did we get too far away during TC?
				return false;
		}
		else if ((*k)->hasAnyType(MoveData::kMVT_Basic)) { // get push from single basics
			if ((*k)->name.find("L") != (*k)->name.npos)
				push += 2;
			if ((*k)->name.find("M") != (*k)->name.npos)
				push += 3;
			if ((*k)->name.find("H") != (*k)->name.npos)
				push -= 1;
		}
		else if ((*k)->hasAnyType(MoveData::kMVT_Dash)) // remove a lot of push for dashes
			push -= 10;
		// remove moderate push for non-projectile specials
		else if((*k)->notAnyType(MoveData::kMVT_Projectile) &&
			(*k)->hasAnyType(MoveData::kMVT_Special | MoveData::kMVT_VS | MoveData::kMVT_CA | MoveData::kMVT_VR))
			push = -6;
		if ((*k)->hasAnyType(MoveData::kMVT_KnockBack)) // account for knockbacks
			push += 10;
	}
	return true;
}