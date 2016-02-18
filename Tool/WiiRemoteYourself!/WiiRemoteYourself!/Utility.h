#pragma once

template<class T>
struct Vector
{
	T X;
	T Y;
	T Z;

	Vector() { X = 0; Y = 0; Z = 0; }
	Vector(T value) { X = Y = Z = value; }
	Vector(T x, T y, T z) { X = x; Y = y; Z = z; }

};

template<class T>
struct Vector2D
{
	T X;
	T Y;

	Vector2D() { X = 0; Y = 0; }
	Vector2D(T value) { X = Y = value; }
	Vector2D(T x, T y) { X = x; Y = y; }

};

template<class T>
struct Rotator
{
	T Pitch;
	T Yaw;
	T Roll;

	Rotator() { Pitch = 0; Yaw = 0; Roll = 0; }
	Rotator(T value) { Pitch = Yaw = Roll = value; }
	Rotator(T pitch, T yaw, T roll) { Pitch = pitch, Yaw = yaw, Roll = roll; }

};

class MathHelper
{

public:

	template<class T>
	static T Sign(const T& value) { return (value < 0) ? T(-1) : T(1); }

	template<class T>
	static T Square(const T& value) { return value * value; }

	template<class T>
	static T Dot(const T& x1, const T& y1, const T& z1, const T& x2, const T& y2, const T& z2) 
	{
		return x1 * x2 + y1 * y2 + z1 * z2;
	}

	template<class T>
	static T Dot(const Vector<T>& v1, const Vector<T>& v2)
	{
		return v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z;
	}

};
