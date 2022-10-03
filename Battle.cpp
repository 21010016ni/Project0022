#include "Battle.hpp"
#include <random>
#include <algorithm>
#include <format>
#include "Log.hpp"
#include "Canvas.hpp"

std::mt19937 random(std::random_device{}());

std::vector<std::weak_ptr<Unit>>& Battle::ShuffledUnit()
{
	static std::vector<std::weak_ptr<Unit>> ret;
	if(ret.size() != unit.size())
	{
		ret.clear();
		for(const auto& i : unit)
			ret.emplace_back(i);
	}
	std::shuffle(ret.begin(), ret.end(), random);
	return ret;
}

void Battle::update()
{
	static int turn = 0;
	Log::push(Log::Tag::battle, std::format("{}�^�[����", ++turn).c_str());
	target.clear();
	auto move = ShuffledUnit();
	for(auto& i : move)
	{
		auto buf = i.lock();

		// ��Ԉُ�̏���
		for(auto& s : buf->state)
		{
			// �s�����ɏ����������Ԉُ�
			switch(s.id())
			{
			case 0:	// ��
				Log::push(Log::Tag::battle, std::format("{}�͓łɐN�����1�̃_���[�W", buf->base->name).c_str());
				buf->value.hp -= 1;
				break;
			}

			// ���ʎ��Ԃ̌���
			--s;
		}

		// �X�e�[�^�X�\��
		Log::push(Log::Tag::battle, std::format("{} HP : {}", buf->base->name, buf->value.hp).c_str());

		// �X�L������
		if((buf->cool -= buf->value.spd) <= 0)
		{
			buf->cool = 0;
			Log::push(Log::Tag::battle, std::format("{}�̍s��", buf->base->name).c_str());
			unsigned char n = 0;
			for(size_t j = 0; j < buf->base->skill.size() && n < 255; ++n)
			{
				Log::push(Log::Tag::battle, std::format("�s{}�t�I", buf->base->skill[j].Name()).c_str());
				buf->cool += buf->base->skill[j](*buf, *this, j);
			}
			Canvas::PlayEffect(0);
			Log::push(Log::Tag::battle, std::format("{}��s�������@CT", (int)n, buf->cool).c_str());
		}
	}
	Log::push(Log::Tag::battle, "�^�[���I��");
	Log::push(Log::Tag::battle, "");
}

std::vector<std::weak_ptr<Unit>>& Battle::get(const Unit& u, const std::function<bool(const Unit&, const Unit&)>& regulation, unsigned short num, unsigned short count)
{
	target.clear();
	while(count-- > 0)
	{
		auto& buf = ShuffledUnit();
		unsigned short i = 0;
		for(auto it = buf.cbegin(); it != buf.cend() && i < num; ++it)
		{
			if(regulation(u, *(it->lock())))
			{
				target.emplace_back(*it);
				++i;
			}
		}
	}
	return target;
}

