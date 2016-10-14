#pragma once
#include "stdafx.h"

struct MoveData {
	enum Constants {
		kMVT_KnockDownRecover = 49, // loss of KD Adv from Recovery
		kMVT_KnockDownRecoverBack = 44, // loss of KD Adv from Back Recovery
		kMDC_CounterHitAdv = 2,
		kMDC_FrameTrapGap = 2,
    kMDC_FallDownFrames = 20,
	};

	enum HitAdvantageType {
		kHAT_Raw,
    kHAT_Juggle,
		kHAT_Recover,
		kHAT_RecoverBack,
	};

	enum MoveTypes {
		kMVT_Nothing = 0,
		kMVT_Basic = 1,
		kMVT_Throw = 2,
		kMVT_Special = 4,
		kMVT_VS = 8,
		kMVT_VT = 16,
		kMVT_VR = 32,
		kMVT_CA = 64,
		kMVT_Air = 128,
		kMVT_TargetCombo = 256,
		kMVT_EX = 512,
		kMVT_AirThrow = 1024,
		kMVT_HardKnockDown = 2048,
		kMVT_KnockBack = 4096,
		kMVT_Dash = 8192,
		kMVT_KnockDown = 16384,
		kMVT_Whiff = 32768,
		kMVT_Reset = 65536,
		kMVT_Jump = 131072,
		kMVT_Projectile = 262144,
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
	inline bool canCancelInto(unsigned int next) const { return (cancels & next) != 0; }
	inline bool canCancelInto(MoveData next) const { return (cancels & next.type) != 0; }

	inline short blockAdv() const { return block_adv ;}
	inline short hitAdv(HitAdvantageType hat_type = kHAT_Juggle) const {
		if (hat_type == kHAT_Raw || this->notType(kMVT_KnockDown | kMVT_KnockBack | kMVT_HardKnockDown | kMVT_Throw | kMVT_AirThrow))
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
	
	inline short blockAdvVTC() const { return vtc_block_adv ;}
	inline short hitAdvVTC(HitAdvantageType hat_type = kHAT_Raw) const {
		if (hat_type == kHAT_Raw || this->notType(kMVT_KnockDown | kMVT_KnockBack | kMVT_HardKnockDown | kMVT_Throw | kMVT_AirThrow))
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
	
	inline bool hasType(unsigned int mask) const { return (type & mask) != 0; }
	inline bool notType(unsigned int mask) const { return (type & mask) == 0; }

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

	inline bool operator<(const MoveData& rhs) const {
		return name < rhs.name;
	}

private:
	short block_adv;
	short hit_adv;
	unsigned int type;
	unsigned int cancels;
	short vtc_block_adv;
	short vtc_hit_adv;
};