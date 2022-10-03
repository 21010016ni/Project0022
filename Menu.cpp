#include "Menu.hpp"

Display Menu::display({0,0}, {400,384});

Point<int> Menu::itemSize(24, 24);
Menu::Item Menu::root(-1, "");

void Menu::SetSEVolume(unsigned char v)
{
	ChangeVolumeSoundMem(v, se[0]);
	ChangeVolumeSoundMem(v, se[1]);
}

void Menu::select(const Point<int>& mouse)
{
	auto m = display.localize(mouse);

	if(m.x >= 0 && m.x < display.siz.x && m.y < display.siz.y)
	{
		auto item = &root;
		// �I�����̈�ԍŏ���y���W���擾
		int top = display.siz.y;
		for(int i = m.x / itemSize.x; (top = __min(top, display.siz.y - itemSize.y * item->size())), i > 0; --i)
		{
			// ���݈ʒu�����I����������
			if(!(*item))
				break;
			// ���̑I�����̈ʒu���擾
			top += itemSize.y * item->select;
			// ���̑I�������擾
			item = &(*item)[item->select];
		}
		// y���W���͈͓��̂Ƃ��̂ݏ���
		if(m.y > top)
		{
			int buf = (m.y - top) / itemSize.y;
			if(buf < item->size())
			{
				// ���̑I�������󂾂����ꍇ�A���֐i�܂Ȃ�
				if(item->select != buf && !(*item)[buf].empty())
				{
					// ���݈ʒu�̑I���ʒu��ݒ�
					item->select = buf;
					// �I���������e�̎��̑I���������Z�b�g����
					(*item)[buf].select = -1;
				}
				// ���ɑI�����Ă���Ƃ����I�񂾏ꍇ�A�I������������
				else
				{
					// ���݈ʒu�̑I��������
					item->select = -1;
				}
				
				// ����SE�Đ�
				PlaySoundMem(se[1], DX_PLAYTYPE_BACK);

				// �I���������e�����֐������s
				(*item)[buf](item->select, (*item)[buf]);
			}
		}
	}
}

void Menu::draw(const Point<int>& mouse)
{
	auto m = display.localize(mouse);
	bool highlight = false;
	static Point<int> prev(-1, -1);
	Point<int> now(-1, (m.x >= 0 && m.x < display.siz.x) ? m.x / itemSize.x : -1);

	Point<int> p(display.siz.y, 0);
	auto i = &root;
	int level = 0;
	while((p.y = __min(p.y, display.siz.y - itemSize.y * i->size())), *i)
	{
		for(int n = 0; n < i->size(); ++n)
		{
			display.DrawBox(p.x, p.y + itemSize.y * n, itemSize, 0x3b3b3b, true);
			display.DrawIcon(p.x, p.y + itemSize.y * n, (*i)[n].icon);
			display.DrawBox(p.x, p.y + itemSize.y * n, itemSize, 0x7a7a7a, false);
		}
		if(i->select != -1)
		{
			SetDrawBlendMode(DX_BLENDMODE_ADD, 63);
			display.DrawBox(p.x, p.y + itemSize.y * i->select, itemSize, 0xffffffff, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		if (!highlight && now.x == level)
		{
			highlight = true;
			if (m.y >= p.y && m.y < p.y + itemSize.y * i->size())
			{
				now.y = (m.y - p.y) / itemSize.y;
				if (now != prev)
				{
					PlaySoundMem(se[0], DX_PLAYTYPE_BACK);
				}
				SetDrawBlendMode(DX_BLENDMODE_ADD, 63);
				display.DrawBox(p.x, p.y + itemSize.y * now.y, itemSize, 0xffffffff, true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			else
			{
				now.y = -1;
			}
		}
		p.y += itemSize.y * i->select;
		p.x += itemSize.x;
		i = &(*i)[i->select];
		++level;
	}
	Point<int> siz = {itemSize.y,display.siz.x - p.x};
	for(int n = 0; n < i->size(); ++n)
	{
		display.DrawBox(p.x, p.y + itemSize.y * n, siz, 0x3b3b3b, true);
		display.DrawIcon(p.x, p.y + itemSize.y * n, (*i)[n].icon);
		display.DrawRawString(p.x + itemSize.x, p.y + itemSize.y * n + 2, (*i)[n].text, 0xffffffff);
		display.DrawBox(p.x, p.y + itemSize.y * n, siz, 0x7a7a7a, false);
	}
	if(!highlight && now.x >= level)
	{
		now.x = level;
		if(m.y >= p.y && m.y < p.y + itemSize.y * i->size())
		{
			now.y = (m.y - p.y) / itemSize.y;
			if (now != prev)
			{
				PlaySoundMem(se[0], DX_PLAYTYPE_BACK);
			}
			SetDrawBlendMode(DX_BLENDMODE_ADD, 63);
			display.DrawBox(p.x, p.y + itemSize.y * now.y, siz, 0xffffffff, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else
		{
			now.y = -1;
		}
	}
	prev = now;
}

