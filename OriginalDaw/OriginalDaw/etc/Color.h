#pragma once
#include <complex>

class Color
{
public:
	// 赤
	float r;

	// 緑
	float g;

	// 青
	float b;

	// アルファ
	float a;

	// コンストラクタ
	Color() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}
	constexpr Color(const float& i) : r(i), g(i), b(i), a(i) {}
	constexpr Color(const float& r, const float& g, const float& b, const float& a) :
		r(r), g(g), b(b), a(a) {}

	// デストラクタ
	~Color() {}

	// オペレータオーバーロード
	Color operator+(void) const {
		return *this;
	}
	Color operator-(void) const {
		return { -r, -g, -b, -a };
	}
	Color operator+(const Color& color) const {
		return { r + color.r, g + color.g, b + color.b, a + color.a };
	}
	Color operator+(const float& i) const {
		return { r + i, g + i, b + i, a + i };
	}
	Color operator-(const Color& color) const {
		return { r - color.r, g - color.g, b - color.b, a - color.a };
	}
	Color operator-(const float& i) const {
		return { r - i, g - i, b - i, a - i };
	}
	Color operator*(const Color& color) const {
		return { r * color.r, g * color.g, b * color.b, a * color.a };
	}
	Color operator*(const float& i) const {
		return { r * i, g * i, b * i, a * i };
	}
	Color operator/(const Color& color) const {
		return { r / color.r, g / color.g, b / color.b, a / color.a };
	}
	Color operator/(const float& i) const {
		return { r / i, g / i, b / i, a / i };
	}
	void operator=(const Color& color) {
		r = color.r;
		g = color.g;
		b = color.b;
		a = color.a;
	}
	void operator=(const float& i) {
		r = i;
		g = i;
		b = i;
		a = i;
	}
	void operator+=(const Color& color) {
		r += color.r;
		g += color.g;
		b += color.b;
		a += color.a;
	}
	void operator+=(const float& i) {
		r += i;
		g += i;
		b += i;
		a += i;
	}
	void operator-=(const Color& color) {
		r -= color.r;
		g -= color.g;
		b -= color.b;
		a -= color.a;
	}
	void operator-=(const float& i) {
		r -= i;
		g -= i;
		b -= i;
		a -= i;
	}
	void operator*=(const Color& color) {
		r *= color.r;
		g *= color.g;
		b *= color.b;
		a *= color.a;
	}
	void operator*=(const float& i) {
		r *= i;
		g *= i;
		b *= i;
		a *= i;
	}
	void operator/=(const Color& color) {
		r /= color.r;
		g /= color.g;
		b /= color.b;
		a /= color.a;
	}
	void operator/=(const float& i) {
		r /= i;
		g /= i;
		b /= i;
		a /= i;
	}
	constexpr bool operator==(const Color& color) const {
		return (r == color.r && g == color.g && b == color.b && a == color.a);
	}
	constexpr bool operator==(const float& i) const {
		return (r == i && g == i && b == i && a == i);
	}
	constexpr bool operator!=(const Color& color) const {
		return !(r == color.r && g == color.g && b == color.b && a == color.a);
	}
	constexpr bool operator!=(const float& i) const {
		return !(r == i && g == i && b == i && a == i);
	}
};

inline Color operator+(const float& i, const Color& color) {
	return { color.r + i, color.g + i, color.b + i, color.a + i };
}

inline Color operator-(const float& i, const Color& color) {
	return { color.r - i, color.g - i, color.b - i, color.a - i };
}

inline Color operator*(const float& i, const Color& color) {
	return { color.r * i, color.g * i, color.b * i, color.a * i };
}

inline Color operator/(const float& i, const Color& color) {
	return { color.r / i, color.g / i, color.b / i, color.a / i };
}
