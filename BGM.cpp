#include "BGM.hpp"
#include <DxLib.h>
#include <random>
#include <algorithm>
#include <stdexcept>
#include "convert_string.hpp"

extern std::mt19937 engine;

void BGM::set(const char8_t* file)
{
	//music.emplace_back(LoadMusicMem(file.c_str()), file.substr(0, file.find_last_of('.')));
	music.emplace_back(file);
}

void BGM::play(size_t id)
{
	if(id >= music.size())
		throw std::out_of_range("out of range");
	// bufをセット
	buf.clear();
	buf.emplace_back(&music[id]);
	// タイトル更新
	title = u8"♪" + music[id].substr(0, music[id].find_last_of('.')).substr(music[id].find_last_of(u8"/\\") + 1);
	// 再生
	PlayMusic(ext::tochar(music[id]), DX_PLAYTYPE_BACK);
	SetVolumeMusic(volume);
}

void BGM::play(const std::u8string& file)
{
	// タイトル更新
	title = u8"♪" + file.substr(0, file.find_last_of('.')).substr(file.find_last_of(u8"/\\") + 1);
	// 再生
	PlayMusic(ext::tochar(file), DX_PLAYTYPE_BACK);
	SetVolumeMusic(volume);
}

void BGM::stop()
{
	StopMusic();
}

bool BGM::update()
{
	// もし曲が再生されていなかったら
	if(CheckMusic() == 0)
	{
		// もしループ再生が有効かつ再生できる曲があるなら
		if(mode & 1 && !music.empty())
		{
			// stackが空だったら補充
			if(stack.empty())
			{
				// shuffleが有効ならshuffle
				if(mode & 2)
				{
					// bufをリセットする
					buf.clear();
					for(const auto& i : music)
						buf.emplace_back(&i);
					std::shuffle(buf.begin(), buf.end(), engine);
				}
				if(!buf.empty())
				{
					// stackを補充
					for(const auto& i : buf)
						stack.emplace(i);
				}
			}
			// タイトル更新
			title = u8"♪" + stack.top()->substr(0, stack.top()->find_last_of('.')).substr(stack.top()->find_last_of(u8"/\\") + 1);
			// 再生
			PlayMusic(ext::tochar(*stack.top()), DX_PLAYTYPE_BACK);
			SetVolumeMusic(volume);
			// スタック削除
			stack.pop();
			// 曲の更新を知らせて終了
			return true;
		}
		// ループが無効、または再生できる曲が無いなら
		else
		{
			// タイトル更新
			title.clear();
			// 曲を更新していないことを知らせて終了
			return false;
		}
	}
	// 曲を更新していないことを知らせて終了
	return false;
}

void BGM::ChangeVolume(unsigned char v)
{
	SetVolumeMusic(v);
	volume = v;
}

