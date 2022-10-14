#include "Field.hpp"
#include <random>

extern std::mt19937 engine;

int Field::speed = 60;

void Field::update()
{
	// ���i����퓬�I������������Ă��Ȃ��̂ŁA��莞�Ԑ�����炻��ŏI������悤�Ɂj
	static int battleCount = 0;

	// �i�s
	if (!pause && ++count >= speed)
	{
		count = 0;
		
		if (battle)
		{
			battle->update();
			if (++battleCount >= 10)
				battle.reset();
		}
		else
		{
			// �퓬���łȂ������ꍇ�A�K���ɉ�b���������܂ɐ퓬�𔭐�������
			
			// �[�x��i�߂�
			++alpha;

			// ���O�̉�b���擾
			auto& prev = Log::latest(Log::talk);

			// �퓬�J�n�̂��
			if (!std::uniform_int_distribution{ 0,19 }(engine))
			{
				// �L�����N�^�[���擾
				auto wu = get(GetMode::ignore, 0xffffffff);
				// �L�����N�^�[�����݂��Ă�����擪�J�n���̑䎌��T���A���݂����炻��𔭌�
				if (!wu.expired())
				{
					auto word = wu.lock()->base->GetWord(Charactor::battle_start, prev);
					if (word)
						Log::push(word);
				}
				battle.reset(new Battle);
				for (auto& i : unit)
				{
					battle->set(i);
				}

				// ��
				battleCount = 0;
			}
			else
			{
				// ��b
				bool talked = false;
				size_t cpos;

				// �������O�̉�b�Ɏ����ȊO�̐F���܂܂�Ă�����A�b�������Ƃ��ď�������
				if (prev && (cpos = prev.text().find_first_of('#', 1)) != std::u8string::npos)
				{
					// �Ώۂ��擾����
					auto wu = get(GetMode::now_in, color(prev.text().substr(cpos, 7)));
					// �Ώۂ����݂��Ă�����i�ށA�����łȂ��Ȃ�I��
					if (!wu.expired())
					{
						// �Ώۂ̑䎌���擾
						auto word = wu.lock()->base->GetWord(Charactor::reaction + color(prev.text().substr(0, 7)), prev);
						// �����Ή�����䎌������΁A����𔭌����ĉ�b�ς݃t���O�𗧂Ă�
						if (word)
						{
							Log::push(word);
							talked = true;
						}
						// �Ή�����䎌�����݂��Ȃ���Ζ��\���A�N�V�������擾�A����������Ȃ甭���҂�ʂ̃L�����N�^�[�ɕς���i�ʏ픭���j
						else
						{
							word = wu.lock()->base->GetWord(Charactor::reaction_any, prev);
							if (word)
							{
								Log::push(word);
								talked = true;
							}
						}
					}
				}
				if (!talked)
				{
					// �L�����N�^�[���擾
					auto unit = get(GetMode::ignore, 0xffffffff);
					// �L�����N�^�[�����݂��Ă�����T�����̑䎌��T���A���݂����炻��𔭌�
					if (!unit.expired())
					{
						auto word = unit.lock()->base->GetWord(Charactor::search, prev);
						if (word)
							Log::push(word);
					}
				}
			}
		}
	}
}

std::weak_ptr<Unit> Field::get(GetMode mode, int color)
{
	static std::vector<std::weak_ptr<Unit>> v;
	if (v.size() != unit.size())
		for (auto& i : unit)
			v.emplace_back(i);
	std::shuffle(v.begin(), v.end(), engine);
	for (auto& i : v)
	{
		switch (mode)
		{
		case Field::GetMode::ignore:
			return i;
		case Field::GetMode::base_ex:
			if ((i.lock()->base->status.color & 0x00ffffff) != color)
				return i;
			break;
		case Field::GetMode::base_in:
			if ((i.lock()->base->status.color & 0x00ffffff) == color)
				return i;
			break;
		case Field::GetMode::now_ex:
			if ((i.lock()->value.color & 0x00ffffff) != color)
				return i;
			break;
		case Field::GetMode::now_in:
			if ((i.lock()->value.color & 0x00ffffff) == color)
				return i;
			break;
		}
	}
	return std::weak_ptr<Unit>();
}

