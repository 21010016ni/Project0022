#pragma once

class Status
{
	// CHADMRSTL
	// BEFGIJKNOPQUVWXYZ
	// 

public:
	Status(int color, int hp, int atk, int def, int mag, int reg, int spd, float tec, float luc)
		:color(color), hp(hp), atk(atk),def(def),mag(mag),reg(reg),spd(spd),tec(tec),luc(luc)
	{
	}

	int hp;

	int color;

	/// <summary>
	/// �����U����
	/// �����n�X�L���̃_���[�W�ɉe������
	/// </summary>
	int atk;

	/// <summary>
	/// �h���
	/// �󂯂�_���[�W���y������
	/// </summary>
	int def;

	/// <summary>
	/// ���@�U����
	/// ���@�n�X�L���̃_���[�W�ɉe������
	/// </summary>
	int mag;

	/// <summary>
	/// ��R��
	/// �󂯂��Ԉُ���y������
	/// </summary>
	int reg;

	/// <summary>
	/// �N�[���^�C���̏������x
	/// �����ނ�10�O������āA�s�x��������
	/// </summary>
	int spd;

	/// <summary>
	/// �m������
	/// </summary>
	float tec;

	/// <summary>
	/// �m������idex�ɉ��Z�j
	/// </summary>
	float luc;
};

