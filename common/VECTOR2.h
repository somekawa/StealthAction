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

	// ������Z�q
	Vector2Temple& operator = (const Vector2Temple& vec);

	// �Y�������Z�q	[ ] �̓A�h���X�v�Z
	T& operator[] (int no);

	// �P�����Z�q(����̋t���O�����Z�q)
	Vector2Temple& operator += (const Vector2Temple& vec);
	Vector2Temple& operator -= (const Vector2Temple& vec);
	Vector2Temple& operator *= (T k);
	Vector2Temple& operator /= (T k);
	Vector2Temple  operator+()const;						// +�����t�^
	Vector2Temple  operator-()const;						// -�����t�^

	// ��r���Z�q
	bool operator == (const Vector2Temple& vec) const;
	bool operator != (const Vector2Temple& vec) const;
	bool operator <= (const Vector2Temple& vec) const;
	bool operator < (const Vector2Temple& vec) const;
	bool operator >= (const Vector2Temple& vec) const;
	bool operator > (const Vector2Temple& vec) const;
};

// �x�N�g���̉��Z(�ǐ�)
// �N���X�̊O�����玩�����g�Ƃ����̂����݂��Ȃ�
// �l�Ŏ󂯎��ƃX�R�[�v�͈͂𔲂���Ƃ��Ƀf�X�g���N�^�������ď����I�ɏd���Ȃ�̂�Vector2Temple��&��t���Ă���
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