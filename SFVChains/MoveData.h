#pragma once
#include "stdafx.h"

struct MoveData {
	enum FlagsAndConstants {
		kFLG_KnockDown = 16384, // mask/flag of highest non-negative bit
		kFLG_Whiff = 8192, // mask/flag of 2nd highest non-negative bit
		kFLG_Reset = 4096, // mask/flag of 3rd highest non-negative bit
		kFLG_KnockDownRecover = -49, // loss of KD Adv from Recovery
		kFLG_KnockDownRecoverBack = -44, // loss of KD Adv from Back Recovery
		kFLG_CounterHitAdv = 2,
	};

	enum MoveTypes {
		kMVT_Nothing = 0,
		kMVT_Basic = 1,
		kMVT_Throw = 2,
		kMVT_Special = 4,
		kMVT_VT = 8,
		kMVT_VS = 8,
		kMVT_VR = 16,
		kMVT_CA = 32,
		kMVT_Air = 64,
		kMVT_TargetCombo = 128,
		kMVT_EX = 256,
	};

	std::string name;
	short startup;
	short block_adv;
	short hit_adv;
	short vtc_block_adv;
	short vtc_hit_adv;
	short damage;
	short stun;
	unsigned short type;
	unsigned short cancels;

	inline bool  isKnockDown()  const { return (hit_adv & kFLG_KnockDown) != 0; }
	inline short knockDownAdv() const { return (hit_adv & ~kFLG_KnockDown); }

	inline bool isWhiffable() const { return (block_adv & kFLG_Whiff) != 0; }
	inline bool isReset() const { return (hit_adv & kFLG_Reset) != 0; }
	inline bool canVTC()  const { return (cancels & kMVT_VT) != 0; }

	inline bool canCancel(MoveTypes next) const { return (cancels & next) != 0; }
	inline bool canCancel(MoveData next) const { return (cancels & next.type) != 0; }

	inline MoveData() :
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
	inline MoveData(
		std::string name,
		short startup,
		short block_adv,
		short hit_adv,
		short vtc_block_adv,
		short vtc_hit_adv,
		short damage,
		short stun,
		unsigned short type,
		unsigned short cancels
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

	inline bool operator<(const MoveData& rhs) const {
		return name < rhs.name;
	}
};