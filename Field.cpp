#include "Field.hpp"
#include <random>

extern std::mt19937 engine;

void Field::update()
{
	// �i�s
	if (!pause && ++count >= speed)
	{
		count = 0;
		
		if (battle)
		{
			battle->update();
		}
		else
		{
			// �퓬���łȂ������ꍇ�A�K���ɉ�b���������܂ɐ퓬�𔭐�������
		}
	}
}

std::weak_ptr<Unit> Field::get(GetMode mode, int color)
{
	if (unit.empty())
		throw nullptr;
	auto it = unit.begin();
	std::advance(it, std::uniform_int_distribution<size_t>{0, unit.size() - 1}(engine));
	return *it;
}

