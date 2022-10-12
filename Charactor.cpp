#include "Charactor.hpp"
#include <format>
#include <random>
#include <boost/algorithm/string.hpp>
#include "string_mb_function.hpp"
#include "Field.hpp"

extern std::mt19937 engine;

Unit::StateSlot* Unit::hasState(int id)
{
	for(int i = 0; i < 4; ++i)
		if(state[i] == id)
			return &state[i];
	return nullptr;
}

void Unit::giveDamage(Unit& t, float m, char type)
{
	// ��b�_���[�W�v�Z
	float v = 0.0f;
	switch(type)
	{
	case 0:	// �C�Ӂi�����E���@�̂��������ق�*�{���j
		v = __max(t.value.atk, t.value.mag) * m;
		break;
	case 1:	// �����i�����U����*�{���j
		v = t.value.atk * m;
		break;
	case 2:	// ���p�i���@�U����*�{���j
		v = t.value.mag * m;
		break;
	case 3:	// �����i�����U����*���@�U����*�{��/2�A�h��͂�������x�����j
		v = (t.value.atk + t.value.mag) * m / 2;
		break;
	case 4:	// �����i�󂯂鑤�̌���HP�̊����j
		v = value.hp * m;
		break;
	}
	// ��������
	float r = std::uniform_real_distribution<float>{t.value.tec, t.value.tec + t.value.luc}(engine);
	v *= r;
	// ��Ԉُ�̏���
	if(t.hasState(10) != nullptr)
		v *= 1.3f;

	// �h��͏���
	switch(type)
	{
	case 3:
		v += value.def / ((t.value.atk + t.value.mag) / 4.0f);
		break;
	default:
		v += value.def;
	}
	if(m > 0)
		v = __max(v, 0);

	// ���f
	value.hp += (int)v;
	// �\��
	Log::push(Log::Tag::battle, std::format("{} HP {}  (<c,{}>{:+}<c>)", base->name, value.hp, (r > 1.2f) ? 0xffffaa : ((v < 0) ? 0xffaaaa : 0xaaffaa), (int)v).c_str());
}

void Unit::giveState(int slot, State* v, int time)
{
	// ���f
	if(v != nullptr)
		state[slot].state = v;
	state[slot] += time;
	// �\��
	Log::push(Log::Tag::battle, std::format("{}<>{}> {}{}", base->name, slot, (state[slot]) ? state[slot].state->Name() : "", state[slot].time).c_str());
}

const std::string* Charactor::GetWord(unsigned int key, const std::string& prev, bool speaker)const
{
	static std::string buf;
	std::vector<std::string> elem;

	for(auto i = word.equal_range(key); i.first != i.second; ++i.first)
	{
		// �L�[���[�h���w�肳��Ă��邩���`�F�b�N
		auto p = ext::find_first_of_mb(i.first->second, '|');
		if(p != std::string::npos)
		{
			// �w�肳��Ă�����A���̈ʒu�܂ł�؂�o����prev�Ɋ܂܂�Ă��邩�������A�܂܂�Ă��Ȃ������玟�̏�����
			if(prev.find(i.first->second.substr(0, p)) == std::string::npos)
				continue;
			buf = i.first->second.substr(p + 1);
		}
		else
		{
			// �w�肳��Ă��Ȃ������ꍇ�A���̂܂�
			buf = i.first->second;
		}

		if (speaker)
		{
			buf.insert(0, std::format("#{:06x}", status.color & 0x00ffffff));
		}

		// ���ꕶ���u��
		while((p = ext::find_first_of_mb(buf, '[')) != std::string::npos)
		{
			auto q = ext::find_first_of_mb(buf, ']', p);
			if(q == std::string::npos)
				throw;
			boost::split(elem, buf.substr(p + 1, q - p - 1), boost::is_any_of(","));
			auto it = elem.begin();
			std::advance(it, std::uniform_int_distribution<size_t>{0, elem.size() - 1}(engine));
			buf.replace(p, q - p + 1, *it);
		}
		while((p = ext::find_first_of_mb(buf,'@')) != std::string::npos)
		{
			int color = 0;
			std::weak_ptr<Unit> unit;
			while (true)
			{
				switch (buf.at(p + 1))
				{
				case 'm':	// �������g�̐F
					color = status.color;
					break;
				case 'p':	// ���O�̔����҂̐F
					if (prev.at(0) == '#')
						color = std::stoi(prev.substr(1, 6), nullptr, 16);
					else
						goto SelectWordRoopEnd;
					break;
				case 'a':	// �t���J���[�̒����烉���_���̐F
					do
					{
						color = std::uniform_int_distribution{ 0x000000,0xffffff }(engine);
					} while (color == (status.color & 0x00ffffff));
					break;
				case 'r':	// ���݂��邷�ׂẴL�����N�^�[�̐F
					// �܂�
					goto SelectWordRoopEnd;
					break;
				case 'f':	// ���݃t�B�[���h��ɂ���L�����N�^�[�̐F�i�ω����Ă����ꍇ�A�{���̐F�j
					unit = Field::get(Field::GetMode::base_ex, status.color);
					if (unit.expired())
						goto SelectWordRoopEnd;
					color = unit.lock()->base->status.color;
					break;
				case 'n':	// ���݃t�B�[���h��ɂ���L�����N�^�[�̐F�i�ω����Ă����ꍇ�A�ω���̐F�j
					unit = Field::get(Field::GetMode::now_ex, status.color);
					if (unit.expired())
						goto SelectWordRoopEnd;
					color = unit.lock()->value.color;
					break;
				case 'e':	// �퓬���������Ă����ꍇ�A���̑���̐F
					// �܂�
					goto SelectWordRoopEnd;
					break;
				}
				break;
			}
			buf.replace(p, 2, std::format("#{:06x}", color & 0x00ffffff));
		}
		return &buf;
	SelectWordRoopEnd:
		continue;
	}
	return nullptr;
}

