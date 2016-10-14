#pragma once
#include "stdafx.h"

struct MoveData {
	enum FlagsAndConstants {
		kFAC_KnockDown = 16384, // mask/flag of highest non-negative bit
		kFAC_Whiff = 8192, // mask/flag of 2nd highest non-negative bit
		kFAC_Reset = 4096, // mask/flag of 3rd highest non-negative bit
		kFAC_KnockDownRecover = -49, // loss of KD Adv from Recovery
		kFAC_KnockDownRecoverBack = -44, // loss of KD Adv from Back Recovery
		kFAC_CounterHitAdv = 2,
		kFAC_FrameTrapGap = 2,
	};

	enum HitAdvantageType {
		kHAT_Raw,
		kHAT_Recover,
		kHAT_RecoverBack,
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
		kMVT_AirThrow = 512,
		kMVT_HardKnockDown = 1024,
		kMVT_KnockBack = 2048,
		kMVT_Dash = 4096,
	};

	std::string name;
	short startup;
	short damage;
	short stun;

	inline bool  isKnockDown(bool hard = false)  const { return (hard)? (hit_adv & kFAC_KnockDown) != 0 || (type & kMVT_HardKnockDown) != 0: (type & kMVT_HardKnockDown) != 0; }

	inline bool isWhiffable() const { return (block_adv & kFAC_Whiff) != 0; }
	inline bool isReset() const { return (hit_adv & kFAC_Reset) != 0; }
	inline bool canVTC()  const { return (cancels & kMVT_VT) != 0; }

	inline bool canCancel() const { return cancels != NULL; }
	inline bool canCancelInto(short next) const { return (cancels & next) != 0; }
	inline bool canCancelInto(MoveData next) const { return (cancels & next.type) != 0; }

	inline short blockAdv() const { return (block_adv & ~kFAC_Whiff) ;}
	inline short hitAdv(HitAdvantageType hat_type = kHAT_Recover) const {
		short time = (hit_adv & ~(kFAC_KnockDown | kFAC_Reset));
		return (hat_type == kHAT_Raw || !(this->isKnockDown() || this->hasType(kMVT_Throw | kMVT_AirThrow)))? // if
				time :
			((hat_type == kHAT_Recover)? // else if
				time - kFAC_KnockDownRecover :
				time - kFAC_KnockDownRecoverBack); // else
	}
	
	inline short blockAdvVTC() const { return (vtc_block_adv & ~kFAC_Whiff) ;}
	inline short hitAdvVTC(HitAdvantageType hat_type = kHAT_Raw) const {
		short time = (vtc_hit_adv & ~(kFAC_KnockDown | kFAC_Reset));
		return (hat_type == kHAT_Raw || !(this->isKnockDown() || this->hasType(kMVT_Throw | kMVT_AirThrow)))? // if
				time :
			((hat_type == kHAT_Recover)? // else if
				time - kFAC_KnockDownRecover :
				time - kFAC_KnockDownRecoverBack); // else
	}
	
	inline bool hasType(short mask) const { return (type & mask) != 0; }
	inline bool notType(short mask) const { return (type & mask) == 0; }

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

private:
	short block_adv;
	short hit_adv;
	unsigned short type;
	unsigned short cancels;
	short vtc_block_adv;
	short vtc_hit_adv;
};