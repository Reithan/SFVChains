#pragma once
#include "stdafx.h"

struct MoveData {
	enum Constants {
		kMVT_KnockDownRecover = -49, // loss of KD Adv from Recovery
		kMVT_KnockDownRecoverBack = -44, // loss of KD Adv from Back Recovery
		kMDC_CounterHitAdv = 2,
		kMDC_FrameTrapGap = 2,
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
		kMVT_KnockDown = 8192,
		kMVT_Whiff = 16384,
		kMVT_Reset = 32768, // this is highest for short - will need to expand to int if more types/flags are needed
	};

	std::string name;
	short startup;
	short damage;
	short stun;

	inline bool  isKnockDown(bool hard = false)  const { return (hard)? (type & kMVT_KnockDown) != 0 || (type & kMVT_HardKnockDown) != 0: (type & kMVT_HardKnockDown) != 0; }

	inline bool isWhiffable() const { return (type & kMVT_Whiff) != 0; }
	inline bool isReset() const { return (type & kMVT_Reset) != 0; }
	inline bool canVTC()  const { return (type & kMVT_VT) != 0; }

	inline bool canCancel() const { return cancels != NULL; }
	inline bool canCancelInto(short next) const { return (cancels & next) != 0; }
	inline bool canCancelInto(MoveData next) const { return (cancels & next.type) != 0; }

	inline short blockAdv() const { return block_adv ;}
	inline short hitAdv(HitAdvantageType hat_type = kHAT_Recover) const {
		return (hat_type == kHAT_Raw || !(this->isKnockDown() || this->hasType(kMVT_Throw | kMVT_AirThrow)))? // if
				hit_adv :
			((hat_type == kHAT_Recover)? // else if
				hit_adv - kMVT_KnockDownRecover :
				hit_adv - kMVT_KnockDownRecoverBack); // else
	}
	
	inline short blockAdvVTC() const { return vtc_block_adv ;}
	inline short hitAdvVTC(HitAdvantageType hat_type = kHAT_Raw) const {
		return (hat_type == kHAT_Raw || !(this->isKnockDown() || this->hasType(kMVT_Throw | kMVT_AirThrow)))? // if
				vtc_hit_adv :
			((hat_type == kHAT_Recover)? // else if
				vtc_hit_adv - kMVT_KnockDownRecover :
				vtc_hit_adv - kMVT_KnockDownRecoverBack); // else
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