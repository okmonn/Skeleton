#pragma once
#include "../Vector2.h"
#include <string>
#include <memory>
#include <functional>
#include <unordered_map>

class Application;
class Sound;
class Mouse;

class ParamMane
{
	// �܂ݏ��
	struct Thumb {
		//�摜ID
		int id;
		//���W
		Vec2f pos;
		//�T�C�Y
		Vec2f size;
		//��]�p�x
		float angle;
		//�ő���ʗ�
		float max;
		//�ŏ����ʗ�
		float min;
		//���ʗ�
		float effect;
	};

	// �t�B���^�^�C�v
	enum class FilterType {
		low,
		band,
		high
	};

	// �t�B���^�^�C�v�ύX�{�b�N�X
	struct FilterBox {
		//���W
		Vec2f pos;
		//�T�C�Y
		Vec2f size;
		//�^�C�v
		FilterType type;
	};

public:
	// �R���X�g���N�^
	ParamMane(std::weak_ptr<Application>app, std::weak_ptr<Sound>sound, std::weak_ptr<Mouse>mouse);
	// �f�X�g���N�^
	~ParamMane();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

private:
	// �摜�ǂݍ���
	void Load(std::string name, const std::string& fileName, const Vec2f& size, const Vec2f& pos = 0.0f);

	// ���ʏ��̃Z�b�g
	void SetEffect(const std::string& name, const float& min, const float& max, const float& init = 0.0f);

	// �{�b�N�X�̃Z�b�g
	void SetBox(const std::string& name, const FilterType& type, const Vec2f& pos, const Vec2f& size);

	// ������
	void Init(void);

	// �֐��|�C���^�̗�
	void InitFunc(void);


	// �A�v���P�[�V����
	std::weak_ptr<Application>app;

	// �T�E���h
	std::weak_ptr<Sound>sound;

	// �}�E�X
	std::weak_ptr<Mouse>mouse;

	// �t�B���^�^�C�v
	FilterType type;

	// �O�̌��ʗ�
	float oldEffe;

	// �܂�
	std::unordered_map<std::string, Thumb>thumb;

	// �t�B���^�ύX�{�b�N�X
	std::unordered_map<std::string, FilterBox>box;

	// �֐��|�C���^
	std::unordered_map<std::string, std::function<void(void)>>func;
};
