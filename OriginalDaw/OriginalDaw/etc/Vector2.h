#pragma once
#include <complex>

template<typename T>
class Vector2
{
public:
	// X成分
	T x;

	// Y成分
	T y;

	// コンストラクタ
	Vector2() : x((T)0), y((T)0) {}
	constexpr Vector2(const T& i) : x(i), y(i) {}
	constexpr Vector2(const T& x, const T& y) : x(x), y(y) {}

	// ベクトルの長さ
	T Length(void) const {
		return (T)std::sqrt(LengthSquare());
	}

	// ベクトルの長さの2乗
	constexpr T LengthSquare(void) const {
		return (Dot(*this));
	}

	// ベクトルとの内積
	constexpr T Dot(const Vector2& vec) const {
		return (x * vec.x + y * vec.y);
	}

	// ベクトルとの距離
	T Distance(const Vector2& vec) const {
		return (vec - *this).Length();
	}

	// ベクトルとのラジアン角度
	T Radian(const Vector2& vec) const {
		return (T)std::acos(Dot(vec) / (Length() * vec.Length()));
	}

	// ベクトルとの弧度法角度
	T Angle(const Vector2& vec) const {
		return (T)((float)Radian(vec) * 180.0f / 3.141592f);
	}

	// 正規化
	Vector2 Normalized(void) const {
		return (*this / Length());
	}

	// ゼロ確認
	constexpr bool IsZero(void) const {
		return (x == (T)0 && y == (T)0);
	}

	// デストラクタ
	~Vector2() {
	}

	// オペレータオーバーロード
	constexpr Vector2 operator+(void) const {
		return *this;
	}
	constexpr Vector2 operator-(void) const {
		return { -x, -y };
	}
	constexpr Vector2 operator+(const Vector2& vec) const {
		return { x + vec.x, y + vec.y };
	}
	constexpr Vector2 operator+(const T& i) const {
		return { x + i, y + i };
	}
	constexpr Vector2 operator-(const Vector2& vec) const {
		return { x - vec.x, y - vec.y };
	}
	constexpr Vector2 operator-(const T& i) const {
		return { x - i, y - i };
	}
	constexpr Vector2 operator*(const Vector2& vec) const {
		return { x * vec.x, y * vec.y };
	}
	constexpr Vector2 operator*(const T& i) const {
		return { x * i, y * i };
	}
	constexpr Vector2 operator/(const Vector2& vec) const {
		return { x / vec.x, y / vec.y };
	}
	constexpr Vector2 operator/(const T& i) const {
		return { x / i, y / i };
	}
	void operator=(const Vector2& vec) {
		x = vec.x;
		y = vec.y;
	}
	void operator=(const T& i) {
		x = i;
		y = i;
	}
	void operator+=(const Vector2& vec) {
		x += vec.x;
		y += vec.y;
	}
	void operator+=(const T& i) {
		x += i;
		y += i;
	}
	void operator-=(const Vector2& vec) {
		x -= vec.x;
		y -= vec.y;
	}
	void operator-=(const T& i) {
		x -= i;
		y -= i;
	}
	void operator*=(const Vector2& vec) {
		x *= vec.x;
		y *= vec.y;
	}
	void operator*=(const T& i) {
		x *= i;
		y *= i;
	}
	void operator/=(const Vector2& vec) {
		x /= vec.x;
		y /= vec.y;
	}
	void operator/=(const T& i) {
		x /= i;
		y /= i;
	}
	constexpr bool operator==(const Vector2& vec) const {
		return (x == vec.x && y == vec.y);
	}
	constexpr bool operator==(const T& i) const {
		return (x == i && y == i);
	}
	constexpr bool operator!=(const Vector2& vec) const {
		return !(x == vec.x && y == vec.y);
	}
	constexpr bool operator!=(const T& i) const {
		return !(x == i && y == i);
	}
};

template<typename T>
inline constexpr Vector2<T> operator+(const T& i, const Vector2<T>& vec) {
	return { vec.x + i, vec.y + i };
}

template<typename T>
inline constexpr Vector2<T> operator-(const T& i, const Vector2<T>& vec) {
	return { vec.x - i, vec.y - i };
}

template<typename T>
inline constexpr Vector2<T> operator*(const T& i, const Vector2<T>& vec) {
	return { vec.x * i, vec.y * i };
}

template<typename T>
inline constexpr Vector2<T>operator/(const T& i, const Vector2<T>& vec) {
	return { vec.x / i, vec.y / i };
}

// Vector2<int>の型変換
typedef Vector2<int>Vec2;
// Vector2<float>の型変換
typedef Vector2<float>Vec2f;
