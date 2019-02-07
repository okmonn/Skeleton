#pragma once
#include "../Vector2.h"
#include <string>
#include <memory>
#include <unordered_map>

class Window;
class List;
class Texture;

class TexMane
{
public:
	// �R���X�g���N�^
	TexMane(std::weak_ptr<Window>win);
	// �f�X�g���N�^
	~TexMane();

	// �ǂݍ���
	void Load(int& addr, const std::string& fileName);

	// �`��
	void Draw(std::weak_ptr<List>list, int& addr, const Vec2f& pos, const Vec2f& size, const Vec2f& uvPos, const Vec2f& uvSize,
		const float& alpha, const float& angle, const bool& turnX, const bool& turnY);

	// �e�N�X�`���̍폜
	void Delete(int& addr);

private:
	// ������
	void Init(void);


	// �E�B���h�E
	std::weak_ptr<Window>win;

	// �摜
	std::unordered_map<int*, std::unique_ptr<Texture>>texture;
};
