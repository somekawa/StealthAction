#pragma once
template <class T> 
class Vector2Temple
{
public:
	Vector2Temple();
	Vector2Temple(T x,T y);
	~Vector2Temple();
	T x;
	T y;

	// 代入演算子
	Vector2Temple& operator = (const Vector2Temple& vec);

	// 添え字演算子	[ ] はアドレス計算
	T& operator[] (int no);

	// 単項演算子(これの逆が三項演算子)
	Vector2Temple& operator += (const Vector2Temple& vec);
	Vector2Temple& operator -= (const Vector2Temple& vec);
	Vector2Temple& operator *= (T k);
	Vector2Temple& operator /= (T k);
	Vector2Temple  operator+()const;						// +符号付与
	Vector2Temple  operator-()const;						// -符号付与

	// 比較演算子
	bool operator == (const Vector2Temple& vec) const;
	bool operator != (const Vector2Temple& vec) const;
	bool operator <= (const Vector2Temple& vec) const;
	bool operator < (const Vector2Temple& vec) const;
	bool operator >= (const Vector2Temple& vec) const;
	bool operator > (const Vector2Temple& vec) const;
};

// ベクトルの演算(可読性)
// クラスの外だから自分自身というのが存在しない
// 値で受け取るとスコープ範囲を抜けるときにデストラクタが走って処理的に重くなるのでVector2Templeに&を付けておく
// VECTOR2 + T 
template <class T>
Vector2Temple<T> operator+(const Vector2Temple<T>& u, const T v);

// VECTOR2 - T
template <class T>
Vector2Temple<T> operator-(const Vector2Temple<T>& u, const T v);

// VECTOR2 * T
template <class T>
Vector2Temple<T> operator*(const Vector2Temple<T>& u, const T v);

// T * VECTOR2
template <class T>
Vector2Temple<T> operator*(const T v, const Vector2Temple<T>& u);

// VECTOR2 / T
template <class T>
Vector2Temple<T> operator/(const Vector2Temple<T>& u, const T v);

// VECTOR2 % T
template <class T>
Vector2Temple<T> operator%(const Vector2Temple<T>& u, const T v);

// VECTOR2 + VECTOR2
template <class T>
Vector2Temple<T> operator+(const Vector2Temple<T>& u, const Vector2Temple<T>& v);

// VECTOR2 - VECTOR2
template <class T>
Vector2Temple<T> operator-(const Vector2Temple<T>& u, const Vector2Temple<T>& v);

using Vector2 = Vector2Temple<int>;
using Vector2Dbl = Vector2Temple<double>;
using Vector2Flt = Vector2Temple<float>;

#include "./details/VECTOR2.h"