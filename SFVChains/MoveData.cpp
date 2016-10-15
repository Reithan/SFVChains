#include "stdafx.h"
#include "MoveData.h"

MoveData::MoveData() :
	name(""),
	startup(0),
	block_adv(0),
	hit_adv(0),
	vtc_block_adv(0),
	vtc_hit_adv(0),
	damage(0),
	stun(0),
	type((MoveTypes)0),
	cancels((MoveTypes)0) {}

MoveData::MoveData(
		std::string name,
		short startup,
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
