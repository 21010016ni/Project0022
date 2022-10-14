#include "Field.hpp"
#include <random>
#include "Particle.hpp"

extern std::mt19937 engine;

int Field::speed = 60;

void Field::update()
{
	//// ���i����퓬�I������������Ă��Ȃ��̂ŁA��莞�Ԑ�����炻��ŏI������悤�Ɂj
	//static int battleCount = 0;

	// �i�s
	if (!pause && ++count >= speed)
	{
		count = 0;
		
		if (battle)
		{
			battle->update();
			//if (++battleCount >= 10)
			//	battle.reset();
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
				auto wu = get();
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

				//// ��
				//battleCount = 0;
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
					auto wu = get(GetMode::in | GetMode::now, color(prev.text().substr(cpos, 7)));
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
					auto unit = get();
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

	if (!pause)
	{
		ParticleSystem::update();
		if (!std::uniform_int_distribution{ 0,59 }(engine))
			ParticleSystem::add<Dust>();
	}
}

std::weak_ptr<Unit> Field::get(unsigned char mode, int color)
{
	static std::vector<std::weak_ptr<Unit>> container;
	if (container.size() != unit.size())
		for (auto& i : unit)
			container.emplace_back(i);
	std::shuffle(container.begin(), container.end(), engine);
	for (auto& i : container)
	{
		if (mode & 1)
		{
			int c = 0x00ffffff;
			if (mode & 4)
				c &= i.lock()->value.color;
			else
				c &= i.lock()->base->status.color;
			if ((c == (color & 0x00ffffff)) == (mode & 2))
				return i;
		}
		else
			return i;
	}
	return std::weak_ptr<Unit>();
}

