#pragma once
#include <memory>
#include <functional>
#include "Charactor.hpp"

class Battle
{
	// ���j�b�g�S�Ă�o�^���Ŋi�[����
	std::vector<std::shared_ptr<Unit>> unit;

	// �V���b�t���������j�b�g��Ԃ�
	std::vector<std::weak_ptr<Unit>>& ShuffledUnit();


public:
	std::vector<std::weak_ptr<Unit>> target;

	void set(Charactor& charactor, Unit::Faction faction)
	{
		unit.emplace_back(new Unit(charactor, faction));
	}
	void update();
	std::vector<std::weak_ptr<Unit>>& get(const Unit&, const std::function<bool(const Unit&, const Unit&)>& regulation, unsigned short num = 1, unsigned short count = 1);
};

