#pragma once
#include "Manager.h"
#include <functional>

class Application;
class Sound;
class Mouse;

class FilterMane : 
	public Manager
{
	enum class FilterType {
		low,
		high
	};
public:
	// �R���X�g���N�^
	FilterMane(std::weak_ptr<Application>app, std::weak_ptr<Sound>sound, std::weak_ptr<Mouse>mouse);
	// �f�X�g���N�^
	~FilterMane();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

	// �J�b�g���g���̎擾
	float GetCut(void) const {
		return cut;
	}

private:
	// ������
	void Init(void);


	// �J�b�g���g��
	float cut;

	// �ߋ��̎��g��
	float oldCut;

	// �t�B���^�^�C�v
	FilterType type;

	// �֐��|�C���^
	std::unordered_map<FilterType, std::function<void(const float&)>>func;
};
