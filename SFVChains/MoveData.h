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
	short recovery;
	short damage;
	short stun;

	inline bool isKnockDown(bool hard = false)  const { return (hard)? ((type & kMVT_HardKnockDown) != 0) : (type & kMVT_KnockDown) != 0 || (type & kMVT_HardKnockDown) != 0; }

	inline bool isWhiffable() const { return (type & kMVT_Whiff) != 0; }
	inline bool isReset() const { return (type & kMVT_Reset) != 0; }
	inline bool canVTC()  const { return (type & kMVT_VT) != 0; }

	inline bool canCancel() const { return cancels != NULL; }
	inline bool canCancelInto(unsigned int next) const { return (cancels & next) != 0; }
	inline bool canCancelInto(MoveData next) const { return (cancels & next.type) != 0; }

	inline short blockAdv(bool cancel = false) const {
		return block_adv - ((cancel)? recovery : 0);}
	inline short hitAdv(HitAdvantageType hat_type = kHAT_Juggle) const { return hitAdv(false, hat_type); }
	short hitAdv(bool cancel, HitAdvantageType hat_type = kHAT_Juggle) const;
	
	inline short blockAdvVTC() const { return vtc_block_adv ;}
	short hitAdvVTC(HitAdvantageType hat_type = kHAT_Raw) const;

	inline bool hasAnyType(unsigned int mask) const { return (type & mask) != 0; }
	inline bool notAnyType(unsigned int mask) const { return (type & mask) == 0; }

	inline bool hasAllTypes(unsigned int mask) const { return (type & mask) == mask; }
	inline bool notAllTypes(unsigned int mask) const { return (type & mask) != mask; }

	MoveData();
	MoveData::MoveData(
		std::string name,
		short startup,
		short recovery,
		short block_adv,
		short hit_adv,
		short vtc_block_adv,
		short vtc_hit_adv,
		short damage,
		short stun,
		unsigned int type,
		unsigned int cancels);

	inline bool operator<(const MoveData& rhs) const { return name < rhs.name; }

private:
	short block_adv;
	short hit_adv;
	unsigned int type;
	unsigned int cancels;
	short vtc_block_adv;
	short vtc_hit_adv;
};

typedef std::list<const MoveData*> Combo;
typedef concurrency::concurrent_queue<Combo> ComboList;
bool CalculateComboMetrics(const Combo& combo, /*out params*/ int& damage, int& stun, int& push, int& EX);
