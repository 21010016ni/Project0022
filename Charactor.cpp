#include "Charactor.hpp"
#include <format>
#include <random>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "convert_string.hpp"
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
	Log::push(Log::Tag::battle, ext::convert(std::format("{} HP {}  (<c,{}>{:+}<c>)", ext::convert(base->name), value.hp, (r > 1.2f) ? 0xffffaa : ((v < 0) ? 0xffaaaa : 0xaaffaa), (int)v)).c_str());
}

void Unit::giveState(int slot, State* v, int time)
{
	// ���f
	if(v != nullptr)
		state[slot].state = v;
	state[slot] += time;
	// �\��
	Log::push(Log::Tag::battle, ext::convert(std::format("{}<>{}> {}{}", ext::convert(base->name), slot, (state[slot]) ? ext::convert(state[slot].state->Name()) : "", state[slot].time)).c_str());
}

const Log::Text Charactor::GetWord(unsigned int key, const Log::Text& prev, bool speaker)const
{
	static std::u8string buf;
	std::vector<std::u8string> elem;

	for(auto i = word.equal_range(key); i.first != i.second; ++i.first)
	{
		// �L�[���[�h���w�肳��Ă��邩���`�F�b�N
		auto p = i.first->second.find_first_of('|');
		if(p != std::u8string::npos)
		{
			// �w�肳��Ă�����A���̈ʒu�܂ł�؂�o����prev�Ɋ܂܂�Ă��邩�������A�܂܂�Ă��Ȃ������玟�̏�����
			if (prev && prev.text().find(i.first->second.substr(0, p)) == std::u8string::npos)
				continue;
			buf = i.first->second.substr(p + 1);
		}
		else
		{
			// �w�肳��Ă��Ȃ������ꍇ�A���̂܂�
			buf = i.first->second;
		}

		// ���ꕶ���u��
		while((p = buf.find_first_of('[')) != std::u8string::npos)
		{
			auto q = buf.find_first_of(']', p);
			if(q == std::u8string::npos)
				throw;
			boost::split(elem, buf.substr(p + 1, q - p - 1), boost::is_any_of(","));
			auto it = elem.begin();
			std::advance(it, std::uniform_int_distribution<size_t>{0, elem.size() - 1}(engine));
			buf.replace(p, q - p + 1, *it);
		}
		while((p = buf.find_first_of('@')) != std::u8string::npos)
		{
			int code = 0;
			std::weak_ptr<Unit> unit;
			while (true)
			{
				switch (buf.at(p + 1))
				{
				case 'm':	// �������g�̐F
					code = status.color;
					break;
				case 'p':	// ���O�̔����҂̐F

					if (prev && prev.HasSpeaker())
						code = color(prev.text().substr(0, 7));
					else
						goto SelectWordRoopEnd;
					break;
				case 'a':	// �t���J���[�̒����烉���_���̐F
					do
					{
						code = std::uniform_int_distribution{ 0x000000,0xffffff }(engine);
					} while (code == (status.color & 0x00ffffff));
					break;
				case 'r':	// ���݂��邷�ׂẴL�����N�^�[�̐F
					// �܂�
					goto SelectWordRoopEnd;
					break;
				case 'f':	// ���݃t�B�[���h��ɂ���L�����N�^�[�̐F�i�ω����Ă����ꍇ�A�{���̐F�j
					unit = Field::get(GetMode::ex | GetMode::base, status.color);
					if (unit.expired())
						goto SelectWordRoopEnd;
					code = unit.lock()->base->status.color;
					break;
				case 'n':	// ���݃t�B�[���h��ɂ���L�����N�^�[�̐F�i�ω����Ă����ꍇ�A�ω���̐F�j
					unit = Field::get(GetMode::ex | GetMode::now, status.color);
					if (unit.expired())
						goto SelectWordRoopEnd;
					code = unit.lock()->value.color;
					break;
				case 'e':	// �퓬���������Ă����ꍇ�A���̑���̐F
					// �܂�
					goto SelectWordRoopEnd;
					break;
				}
				break;
			}
			buf.replace(p, 2, color(code));
		}
		if (speaker)
		{
			buf.insert(0, color(status.color));
		}

		return Log::Text(Log::talk, buf);

	SelectWordRoopEnd:
		continue;
	}
	return Log::Text();
}

Charactor Charactor::load(const char* FileName)
{
	std::ifstream ifs(FileName, std::ios::in | std::ios::binary);
	if (!ifs.is_open())
		throw std::invalid_argument("faiilure open file");
}

void Charactor::output(const std::string& Directory)
{
	std::ofstream ofs(Directory + ext::convert(color(status.color).substr(1)), std::ios::out | std::ios::binary | std::ios::trunc);
	if (!ofs.is_open())
		throw std::invalid_argument("faiilure create file");
	
	// �����Ƀf�[�^���������݂���L�q
}

