#pragma once

class Color
{
public:
	// コンストラクタ
	Color() {
	}
	Color(const float& i) : 
		r(i), g(i), b(i), a(i){
	}
	Color(const float& r, const float& g, const float& b, const float& a) :
		r(r), g(g), b(b), a(a) {
	}
	// デストラクタ
	~Color() {
	}

	// 赤
	float r;
	
	// 緑
	float g;

	// 青
	float b;

	// アルファ
	float a;


	// 演算子のオーバーロード
	void operator=(const Color& color) {
		r = color.r;
		g = color.g;
		b = color.b;
		a = color.a;
	}
};
