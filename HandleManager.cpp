#include "HandleManager.hpp"
#include <DxLib.h>

int HandleManager::value = 3600;

void HandleManager::update()
{
	for (auto i = handle.cbegin(); i != handle.cend();)
	{
		if (!(--(*i->second))())
		{
			i = handle.erase(i);
			continue;
		}
		++i;
	}
}

int HandleManager::get(const std::string& key, Type type)
{
	auto i = handle.find(key);
	if (i == handle.cend())
	{
		int buf = -1;
		if (type != Type::undefined)
		{
			switch (type)
			{
			case Type::graph:
				buf = LoadGraph(key.c_str());
				break;
			case Type::sound:
				buf = LoadSoundMem(key.c_str());
				break;
			}
			handle.emplace(key, new Data(type, buf, value));
		}
		return buf;
	}
	else
	{
		i->second->set(value);
		return i->second->get();
	}
}

HandleManager::Data::~Data()
{
	switch (type)
	{
	case Type::graph:
		DeleteGraph(handle);
		break;
	case Type::sound:
		DeleteSoundMem(handle);
		break;
	}
}

