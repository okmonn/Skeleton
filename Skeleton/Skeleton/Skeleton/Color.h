#pragma once

class Color
{
public:
	// �R���X�g���N�^
	Color() {
	}
	Color(const float& i) : 
		r(i), g(i), b(i), a(i){
	}
	Color(const float& r, const float& g, const float& b, const float& a) :
		r(r), g(g), b(b), a(a) {
	}
	// �f�X�g���N�^
	~Color() {
	}

	// ��
	float r;
	
	// ��
	float g;

	// ��
	float b;

	// �A���t�@
	float a;


	// ���Z�q�̃I�[�o�[���[�h
	void operator=(const Color& color) {
		r = color.r;
		g = color.g;
		b = color.b;
		a = color.a;
	}
};
