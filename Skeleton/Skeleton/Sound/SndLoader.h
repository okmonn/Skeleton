#pragma once
#include "SndFunc.h"
#include <memory>
#include <functional>
#include <unordered_map>

class SndLoader
{
public:
	// �f�X�g���N�^
	~SndLoader();

	// �C���X�^���X�ϐ��̎擾
	static SndLoader& Get(void) {
		static SndLoader instance;
		return instance;
	}

	// �ǂݍ���
	int Load(const std::string& fileName);

	// �T�E���h���̎擾
	snd::Info GetInfo(const std::string& fileName);

	// �T�E���h�f�[�^�̎擾
	std::shared_ptr<std::vector<float>> GetData(const std::string& fileName);

	// �폜
	void Delete(const std::string& fileName);

private:
	// �R���X�g���N�^
	SndLoader();
	SndLoader(const SndLoader&) = delete;
	void operator=(const SndLoader&) = delete;

	// ������
	void Init(void);


	// �T�E���h���
	std::unordered_map<std::string, snd::Info>info;

	// �T�E���h�f�[�^
	std::unordered_map<std::string, std::shared_ptr<std::vector<float>>>data;

	// �֐��|�C���^
	std::unordered_map<int, std::function<void(std::vector<float>& tmp, FILE* file)>>load;
};
