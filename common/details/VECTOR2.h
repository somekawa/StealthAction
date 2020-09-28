template <class T>
Vector2Temple<T>::Vector2Temple()
{
	x = 0;
	y = 0;
}

template <class T>
Vector2Temple<T>::Vector2Temple(T x, T y)
{
	// this�|�C���^�ŏ������Ƃ�
	//this->x = x;
	//this->y = y;

	// ���O��Ԃŏ������Ƃ�
	Vector2Temple<T>::x = x;
	Vector2Temple<T>::y = y;
}

template <class T>
Vector2Temple<T>::~Vector2Temple()
{
}

// ������Z�q
template <class T>
Vector2Temple<T>& Vector2Temple<T>::operator=(const Vector2Temple<T>& vec)
{
	this->x = vec.x;
	this->y = vec.y;
	return (*this);
}

// �Y�������Z�q
template <class T>
T& Vector2Temple<T>::operator[](int no)
{
	if (no == 0)
	{
		return x;
	}
	else if (no == 1)
	{
		return y;
	}
	else
	{
		// ����̒l��Ԃ��āA�G���[�ł���Ƃ����̂��킩��悤�ɂ��Ă�����(-1�Ƃ�)
		return x;
	}
}

// �P�����Z�q
template <class T>
Vector2Temple<T>& Vector2Temple<T>::operator+=(const Vector2Temple<T>& vec)
{
	(this->x += vec.x);
	(this->y += vec.y);
	return (*this);
}

template <class T>
Vector2Temple<T>& Vector2Temple<T>::operator-=(const Vector2Temple<T>& vec)
{
	(this->x -= vec.x);
	(this->y -= vec.y);
	return (*this);
}

template <class T>
Vector2Temple<T>& Vector2Temple<T>::operator*=(T k)
{
	(this->x *= k);
	(this->y *= k);
	return (*this);
}

template <class T>
Vector2Temple<T>& Vector2Temple<T>::operator/=(T k)
{
	(this->x /= k);
	(this->y /= k);
	return (*this);
}

// �󂯎�����l(�������g)�̕�����+-���鏈��
// ����const��t���邱�Ƃɂ���Ă��̊֐����ł̑S�Ă̒l�̏����������s�\�ɂȂ�
template <class T>
Vector2Temple<T>Vector2Temple<T>::operator+() const
{
	// ���̂�Ԃ������ł���
	return (*this);
	//return Vector2Temple(this->x, this->y);
}

template <class T>
Vector2Temple<T>Vector2Temple<T>::operator-() const
{
	// �����o�ϐ��͍��Ȃ����ǃ��[�J���ϐ��͍���
	// Vector2Temple()�͈����Ȃ��̃R���X�g���N�^�����[�J���ϐ��Ƃ��č���Ă���̂Ɠ����Ӗ�
	// �ł����̂܂܂���X��Y��0�ɂȂ��Ă��܂������������̃R���X�g���N�^�����[�J���ϐ��Ƃ��ČĂяo���悤�ɂ���

	// ���[�J���ϐ���return�Ȃ̂ŎQ�ƕԂ����Ă͂����Ȃ����j������邩��
	return Vector2Temple<T>(-this->x, -this->y);
}

// ��r���Z�q
template <class T>
bool Vector2Temple<T>::operator==(const Vector2Temple<T>& vec) const
{
	//if ((this->x == vec.x) && (this->y == vec.y))
	//{
	//	return true;
	//}
	//else
	//{
	//	return false;
	//}

	// �����̉��Z�q�́A�����Ƃ���1 �Ⴄ�Ƃ���0 �ɂȂ�
	// (this->x == vec.x)
	// !���̉��Z�q�́A �����Ƃ���0 �Ⴄ�Ƃ���1 �ɂȂ�
	// (this->x != vec.x)

	return ((this->x == vec.x) && (this->y == vec.y));
}

template <class T>
bool Vector2Temple<T>::operator!=(const Vector2Temple<T>& vec) const
{
	return !((this->x == vec.x) && (this->y == vec.y));
	//return ((this->x != vec.x) && (this->y != vec.y));
}

template <class T>
bool Vector2Temple<T>::operator<=(const Vector2Temple<T>& vec) const
{
	return ((this->x <= vec.x) && (this->y <= vec.y));
}

template <class T>
bool Vector2Temple<T>::operator<(const Vector2Temple<T>& vec) const
{
	return ((this->x < vec.x) && (this->y < vec.y));
}

template <class T>
bool Vector2Temple<T>::operator>=(const Vector2Temple<T>& vec) const
{
	return ((this->x >= vec.x) && (this->y >= vec.y));
}

template <class T>
bool Vector2Temple<T>::operator>(const Vector2Temple<T>& vec) const
{
	return ((this->x > vec.x) && (this->y > vec.y));
}

// �x�N�g���̉��Z
	// VECTOR2 + int 
template <class T>
Vector2Temple<T> operator+(const Vector2Temple<T>& u, const T v)
{
	//Vector2Temple vec;
	//vec.x = u.x + v;
	//vec.y = u.y + v;
	//return vec;
	return Vector2Temple<T>(u.x + v , u.y + v);
}

	// VECTOR2 - int 
template <class T>
Vector2Temple<T> operator-(const Vector2Temple<T>& u, const T v)
{
	return Vector2Temple<T>(u.x - v, u.y - v);
}

	// VECTOR2 * int 
template <class T>
Vector2Temple<T> operator*(const Vector2Temple<T>& u, const T v)
{
	return Vector2Temple<T>(u.x * v, u.y * v);
}

	// int * VECTOR2 
template <class T>
Vector2Temple<T> operator*(const T v, const Vector2Temple<T>& u)
{
	return Vector2Temple<T>(v * u.x, v * u.y);
}

	// VECTOR2 / int 
template <class T>
Vector2Temple<T> operator/(const Vector2Temple<T>& u, const T v)
{
	return Vector2Temple<T>(u.x / v, u.y / v);
}

	// VECTOR2 % int 
template <class T>
Vector2Temple<T> operator%(const Vector2Temple<T>& u, const T v)
{
	return Vector2Temple<T>(u.x % v, u.y % v);
}

	// VECTOR2 + VECTOR2 
template <class T>
Vector2Temple<T> operator+(const Vector2Temple<T>& u, const Vector2Temple<T>& v)
{
	return Vector2Temple<T>(u.x + v.x, u.y + v.y);
}

	// VECTOR2 - VECTOR2 
template <class T>
Vector2Temple<T> operator-(const Vector2Temple<T>& u, const Vector2Temple<T>& v)
{
	return Vector2Temple<T>(u.x - v.x, u.y - v.y);
}
