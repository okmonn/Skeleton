#pragma once
#include "VmdData.h"
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

class Vmd
{
public:
	// �f�X�g���N�^
	~Vmd();

	// �C���X�^���X�ϐ��̎擾
	static Vmd& Get(void) {
		static Vmd instance;
		return instance;
	}

	// �ǂݍ���
	int Load(const std::string& fileName);

	// ���[�V�����f�[�^�̎擾
	std::shared_ptr<std::map<std::string, std::vector<vmd::Motion>>> GetMotion(const std::string& fileName) {
		return motion[fileName];
	}

	// �A�j���[�V�����̎��Ԃ̎擾
	float GetAnimTime(const std::string& fileName) {
		return animTime[fileName];
	}

private:
	// �R���X�g���N�^
	Vmd();
	Vmd(const Vmd&) = delete;
	void operator=(const Vmd&) = delete;

	// ���[�V�������
	std::unordered_map<std::string, std::shared_ptr<std::map<std::string, std::vector<vmd::Motion>>>>motion;

	// �A�j���[�V�����̎���
	std::unordered_map<std::string, float>animTime;
};
