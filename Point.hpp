#pragma once
#include <compare>
#include <cmath>

//--------------------------------
// 2éüå≥ç¿ïWÉNÉâÉX
// v_1.05
//--------------------------------
template<typename T> struct Point
{
	T y, x;
	constexpr Point(T v = 0)noexcept :y(v), x(v) {};
	constexpr Point(T y, T x)noexcept :y(y), x(x) {};
	~Point() {}
	constexpr Point operator+(const Point& t)const noexcept { return { y + t.y,x + t.x }; }
	constexpr Point operator-(const Point& t)const noexcept { return { y - t.y,x - t.x }; }
	constexpr Point operator*(const Point& t)const noexcept { return { y * t.y,x * t.x }; }
	constexpr Point operator/(const Point& t)const { return { y / t.y,x / t.x }; }
	constexpr Point operator*(const T& t)const noexcept { return { y * t,x * t }; }
	constexpr Point operator/(const T& t)const { return { y / t,x / t }; }
	Point& operator+=(const Point& t)noexcept { return y += t.y, x += t.x, *this; }
	Point& operator-=(const Point& t)noexcept { return y -= t.y, x -= t.x, *this; }
	Point& operator*=(const Point& t)noexcept { return y *= t.y, x *= t.x, *this; }
	Point& operator/=(const Point& t) { return y /= t.y, x /= t.x, *this; }
	Point& operator*=(const T& t)noexcept { return y *= t, x *= t, *this; }
	Point& operator/=(const T& t) { return y /= t, x /= t, *this; }
	constexpr auto operator<=>(const Point&)const noexcept = default;
	constexpr Point operator()(const T& t)const noexcept { return { t / x,t % x }; }
	constexpr Point approximate(const Point& siz, int level)const { return { y / (siz.y / level),x / (siz.x / level) }; }
	template<typename U>constexpr U length()const noexcept { return std::sqrt(y * y + x * x); }
	template<typename U>constexpr U distance(Point t)const noexcept { return (t -= *this).length<U>(); }
	template<typename U>constexpr Point& rotate(const U& t)noexcept { return y = y * cos(t) + x * sin(t), x = -y * sin(t) + x * cos(t), *this; }
	template<typename U>constexpr U cross_check(const Point& t, const U& ang)const { return (y - t.y) * cos(ang) - (x - t.x) * sin(ang); }
	template<typename U>constexpr explicit operator Point<U>()const noexcept { return { (U)y,(U)x }; };
	//template <typename U> constexpr explicit operator U()const noexcept { return ((U)y << sizeof(T) * 8) | x; }
};

