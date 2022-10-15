#include "Battle.hpp"
#include <random>
#include <algorithm>
#include <format>
#include "Log.hpp"
#include "Canvas.hpp"
#include "DataBase.hpp"
#include "convert_string.hpp"

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
	// �^�[�����i���j
	static int turn = 0;

	Log::push(Log::Tag::battle, (ext::convert(std::format("{}", ++turn)) + u8"�^�[����").c_str());
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
				Log::push(Log::Tag::battle, (ext::convert(std::format("{}", ext::convert(buf->base->name))) + u8"�͓łɐN�����1�̃_���[�W").c_str());
				buf->value.hp -= 1;
				break;
			}

			// ���ʎ��Ԃ̌���
			--s;
		}

		// �X�e�[�^�X�\��
		Log::push(Log::Tag::battle, ext::convert(std::format("{} HP : {}", ext::convert(buf->base->name), buf->value.hp)).c_str());

		// �X�L������
		if((buf->cool -= buf->value.spd) <= 0)
		{
			buf->cool = 0;
			Log::push(Log::Tag::battle, (ext::convert(std::format("{}", ext::convert(buf->base->name))) + u8"�̍s��").c_str());

			// �G�t�F�N�g���X�g
			std::vector<int> effects;
			// �X�L���������i�Ȃ񂩂킯�킩���X�L��������肵�ĕςȂ��ƂɂȂ������������I��255��X�L������������I���j
			unsigned char n = 0;

			for(unsigned int j = 0; j < buf->base->skill.size() && n < 255; ++n)
			{
				// �X�L���O����
				auto num = j;
				auto& pw = Log::latest(Log::talk);
				auto word = buf->base->GetWord(Charactor::skill_prev + num, pw);
				if(word)
					Log::push(word);

				// �X�L������
				Log::push(Log::Tag::battle, (u8"�s" + ext::convert(std::format("{}", ext::convert(DataBase::skill[buf->base->skill[num].first].Name()))) + u8"�t�I").c_str());
				buf->cool += DataBase::skill[buf->base->skill[num].first](*buf, *this, j);
				// �G�t�F�N�g�o�^
				if (buf->base->skill[num].second != -1)
					effects.emplace_back(buf->base->skill[num].second);

				// �X�L���㔭��
				word = buf->base->GetWord(Charactor::skill_after + num, pw);
				if(word)
					Log::push(word);
			}
			// �G�t�F�N�g�Đ��i�g�p�����X�L���̒����烉���_���j
			if (effects.size() > 0)
			{
				auto it = effects.begin();
				std::advance(it, std::uniform_int_distribution<size_t>{0, effects.size() - 1}(random));
				Canvas::PlayEffect(*it);
			}

			// �s���I�������o��
			Log::push(Log::Tag::battle, (ext::convert(std::format("{}", (int)n)) + u8"��s�������@" + ext::convert(std::format("CT{}", buf->cool))).c_str());
		}
	}
	Log::push(Log::Tag::battle, u8"�^�[���I��");
	Log::push(Log::Tag::battle, u8"");
}

std::vector<std::weak_ptr<Unit>>& Battle::get(const Unit& u, const std::function<bool(const Unit&, const Unit&)>& regulation, unsigned short num, unsigned short count, bool overload)
{
	std::vector<std::weak_ptr<Unit>>* table;
	if(overload)
	{
		table = &target;
	}
	else
	{
		static std::vector<std::weak_ptr<Unit>> over;
		table = &over;
	}

	table->clear();
	while(count-- > 0)
	{
		auto& buf = ShuffledUnit();
		unsigned short i = 0;
		for(auto it = buf.cbegin(); it != buf.cend() && i < num; ++it)
		{
			if(regulation(u, *(it->lock())))
			{
				table->emplace_back(*it);
				++i;
			}
		}
	}
	return *table;
}

