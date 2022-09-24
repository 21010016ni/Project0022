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
	/// 物理攻撃力
	/// 物理系スキルのダメージに影響する
	/// </summary>
	int atk;

	/// <summary>
	/// 防御力
	/// 受けるダメージを軽減する
	/// </summary>
	int def;

	/// <summary>
	/// 魔法攻撃力
	/// 魔法系スキルのダメージに影響する
	/// </summary>
	int mag;

	/// <summary>
	/// 抵抗力
	/// 受ける状態異常を軽減する
	/// </summary>
	int reg;

	/// <summary>
	/// クールタイムの消化速度
	/// おおむね10前後を見て、都度調整する
	/// </summary>
	int spd;

	/// <summary>
	/// 確率下限
	/// </summary>
	float tec;

	/// <summary>
	/// 確率上限（dexに加算）
	/// </summary>
	float luc;
};

