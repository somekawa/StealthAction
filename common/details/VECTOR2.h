template <class T>
Vector2Temple<T>::Vector2Temple()
{
	x = 0;
	y = 0;
}

template <class T>
Vector2Temple<T>::Vector2Temple(T x, T y)
{
	// thisポインタで書いたとき
	//this->x = x;
	//this->y = y;

	// 名前空間で書いたとき
	Vector2Temple<T>::x = x;
	Vector2Temple<T>::y = y;
}

template <class T>
Vector2Temple<T>::~Vector2Temple()
{
}

// 代入演算子
template <class T>
Vector2Temple<T>& Vector2Temple<T>::operator=(const Vector2Temple<T>& vec)
{
	this->x = vec.x;
	this->y = vec.y;
	return (*this);
}

// 添え字演算子
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
		// 特定の値を返して、エラーであるというのがわかるようにしてもいい(-1とか)
		return x;
	}
}

// 単項演算子
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

// 受け取った値(自分自身)の符号を+-する処理
// 後ろにconstを付けることによってこの関数内での全ての値の書き換えが不可能になる
template <class T>
Vector2Temple<T>Vector2Temple<T>::operator+() const
{
	// 実体を返すだけでいい
	return (*this);
	//return Vector2Temple(this->x, this->y);
}

template <class T>
Vector2Temple<T>Vector2Temple<T>::operator-() const
{
	// メンバ変数は作れないけどローカル変数は作れる
	// Vector2Temple()は引数なしのコンストラクタをローカル変数として作っているのと同じ意味
	// でもそのままだとXもYも0になってしまうから引数ありのコンストラクタをローカル変数として呼び出すようにする

	// ローカル変数でreturnなので参照返ししてはいけない→破棄されるから
	return Vector2Temple<T>(-this->x, -this->y);
}

// 比較演算子
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

	// ＝＝の演算子は、同じときは1 違うときは0 になる
	// (this->x == vec.x)
	// !＝の演算子は、 同じときは0 違うときは1 になる
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

// ベクトルの演算
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
