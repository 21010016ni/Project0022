#include "BGM.hpp"
#include <DxLib.h>
#include <random>
#include <algorithm>

extern std::mt19937 engine;

void BGM::set(const char* file)
{
	//music.emplace_back(LoadMusicMem(file.c_str()), file.substr(0, file.find_last_of('.')));
	music.emplace_back(file);
}

bool BGM::update()
{
	if(!music.empty() && CheckMusic() == 0)
	{
		if(!stack.empty())
			stack.pop();
		if(stack.empty())
		{
			std::shuffle(music.begin(), music.end(), engine);
			for(const auto& i : music)
			{
				stack.emplace(&i);
			}
		}
		PlayMusic(stack.top()->c_str(), DX_PLAYTYPE_BACK);
		SetVolumeMusic(volume);
		title = "��" + stack.top()->substr(0, stack.top()->find_last_of('.')).substr(stack.top()->find_last_of("/\\") + 1);
		return true;
	}
	return false;
}

void BGM::ChangeVolume(unsigned char v)
{
	SetVolumeMusic(v);
	volume = v;
}

