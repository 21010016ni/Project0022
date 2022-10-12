#pragma once
#include "Charactor.hpp"
#include "Battle.hpp"

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

	enum class GetMode :char
	{
		ignore,
		base_ex,
		base_in,
		now_ex,
		now_in,
	};

	static std::weak_ptr<Unit> get(GetMode mode, int color);
	static constexpr size_t UnitNum()noexcept { return unit.size(); }
};

