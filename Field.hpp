#pragma once
#include "Charactor.hpp"
#include "Battle.hpp"

class Field
{
	static inline int count = 0;	// 更新カウント

	static inline std::vector<std::shared_ptr<Unit>> unit;	// 探索中のユニット

	static inline std::unique_ptr<Battle> battle;

public:
	static inline int alpha = 0;	// 深度
	static inline bool pause = false;	// 更新の一時停止

	static int speed;	// 更新スピード

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

