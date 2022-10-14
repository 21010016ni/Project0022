#pragma once
#include "Charactor.hpp"
#include "Battle.hpp"

namespace GetMode
{
	unsigned char ex = 0x01;
	unsigned char in = 0x03;
	unsigned char base = 0x00;
	unsigned char now = 0x04;
}

class Field
{
	static inline int count = 0;	// �X�V�J�E���g

	static inline std::vector<std::shared_ptr<Unit>> unit;	// �T�����̃��j�b�g

	static inline std::unique_ptr<Battle> battle;

public:
	static inline int alpha = 0;	// �[�x
	static inline bool pause = false;	// �X�V�̈ꎞ��~

	static int speed;	// �X�V�X�s�[�h

	static void CountReset()
	{
		count = 0;
	}

	static void set(Charactor& charactor, Unit::Faction faction)
	{
		unit.emplace_back(new Unit(charactor, faction));
	}

	static void update();

	static std::weak_ptr<Unit> get(unsigned char mode = 0, int color = 0);
	static constexpr size_t UnitNum()noexcept { return unit.size(); }
};

