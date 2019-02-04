#pragma once
#include "../Vector2.h"
#include <string>
#include <memory>
#include <unordered_map>

class Application;
class Sound;
class Mouse;

class Manager
{
	struct Imag {
		//ID
		int id;
		//UV���W
		Vec2f pos;
		//UV�T�C�Y
		Vec2f size;
		//�p�x
		float angle;
	};
public:
	// �R���X�g���N�^
	Manager();
	// �f�X�g���N�^
	virtual ~Manager();

	// �`��
	virtual void Draw(void) = 0;

	// ����
	virtual void UpData(void) = 0;

	// ���W�̃Z�b�g
	void SetPos(const Vec2f& pos) {
		this->pos = pos;
	}
	// �T�C�Y�̃Z�b�g
	void SetSize(const Vec2f& size) {
		this->size = size;
	}

protected:
	// �ǂݍ���
	void Load(const std::string& name, const std::string& fileName, const Vec2f& uvSize, const Vec2f& uvPos = 0.0f);

	// �`��
	void DrawImg(const std::string& name, const Vec2f& pos, const Vec2f& size, const float& alpha = 1.0f);

	// �}�E�X���͈͂ɓ����Ă��邩�`�F�b�N
	bool CheckMouse(const Vec2f& pos, const Vec2f& size);

	// �p�x�̍X�V
	void UpDataAngle(const std::string& name, const float& now, const float& max);

	// �I��
	void End(void);


	// �A�v���P�[�V����
	std::weak_ptr<Application>app;

	// �T�E���h
	std::weak_ptr<Sound>sound;

	// �}�E�X
	std::weak_ptr<Mouse>mouse;

	// ���W
	Vec2f pos; 

	// �T�C�Y
	Vec2f size;

	// �摜���
	std::unordered_map<std::string, Imag>imag;
};
