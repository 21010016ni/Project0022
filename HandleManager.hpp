#pragma once
#include <memory>
#include <string>
#include <map>

class HandleManager
{
public:
	enum class Type :char
	{
		undefined,
		graph,
		sound,
	};

private:
	struct Data
	{
		const Type type;
		const int handle;
		int duration;
	public:
		Data(Type type, int handle, int duration) :type(type), handle(handle), duration(duration) {}
		Data& operator--()noexcept { return --duration, * this; }
		constexpr bool operator()()const { return duration > 0; }
		void set(int value)noexcept { duration = value; }
		constexpr int get()const noexcept { return handle; }
		~Data();
	};
	static inline std::map<std::u8string, std::unique_ptr<Data>> handle;
	static int value;

public:
	static void update();
	static int get(const std::u8string& key, Type type = Type::undefined);
};

