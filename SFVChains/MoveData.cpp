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

short MoveData::hitAdv(HitAdvantageType hat_type) const {
	if (hat_type == kHAT_Raw || this->notAllTypes(kMVT_KnockDown | kMVT_KnockBack | kMVT_HardKnockDown | kMVT_Throw | kMVT_AirThrow))
		return hit_adv;
	else if (hat_type == kHAT_Juggle)
		return hit_adv - kMVT_KnockDownRecover - kMDC_FallDownFrames;
	else if (hat_type == kHAT_Recover)
		return hit_adv - kMVT_KnockDownRecover;
	else if (hat_type == kHAT_RecoverBack)
		return hit_adv - kMVT_KnockDownRecoverBack;
	else
		return hit_adv;
}

short MoveData::hitAdvVTC(HitAdvantageType hat_type) const {
	if (hat_type == kHAT_Raw || this->notAllTypes(kMVT_KnockDown | kMVT_KnockBack | kMVT_HardKnockDown | kMVT_Throw | kMVT_AirThrow))
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
		if ((*k)->hasAnyType(MoveData::kMVT_EX))
			++EX;
		if ((*k)->hasAnyType(MoveData::kMVT_TargetCombo)) {
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
		else if ((*k)->hasAnyType(MoveData::kMVT_Basic)) {
			if ((*k)->name.find("L") != (*k)->name.npos)
				push += 2;
			if ((*k)->name.find("M") != (*k)->name.npos)
				push += 3;
			if ((*k)->name.find("H") != (*k)->name.npos)
				push -= 1;
		}
		else
			push = 0;
		if (((*k)->hasAnyType(MoveData::kMVT_Basic) && push > 5) ||
			(EX < 0))
			return false;
	}
	return true;
}