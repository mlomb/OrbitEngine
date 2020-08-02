#ifndef RECT_INL
#define RECT_INL

#include "Rect.hpp"

namespace OrbitEngine { namespace Math {
	template<typename T> inline Rect<T>::Rect(const T& _x, const T& _y, const T& _width, const T& _height) : x(_x), y(_y), width(_width), height(_height) { }
	template<typename T> inline Rect<T>::Rect(const Vec2<T> _position, const Vec2<T> _size) : position(_position), size(_size) { }
	template<typename T> inline Rect<T>::Rect() : x(0), y(0), width(0), height(0) { }
} }

#endif