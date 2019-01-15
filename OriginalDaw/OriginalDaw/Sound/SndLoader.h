#pragma once
#include "SndFunc.h"
#include <mutex>
#include <thread>
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

	// �폜
	void Delete(const std::string& fileName);

	// �T�E���h���̎擾
	snd::Snd GetSnd(const std::string& fileName) {
		return sound[fileName];
	}

private:
	// �R���X�g���N�^
	SndLoader();
	SndLoader(const SndLoader&) = delete;
	void operator=(const SndLoader&) = delete;

	// �ǂݍ��݃e�[�u���̍쐬
	void Create(void);

	// �񓯊��ǂݍ���
	void Stream(const std::string& fileName);


	// �X���b�h�t���O
	bool flag;

	// �~���[�e�b�N�X
	std::mutex mtx;

	// �T�E���h���
	std::unordered_map<std::string, snd::Snd>sound;

	// �X���b�h
	std::vector<std::thread>th;

	// �ǂݍ��݃e�[�u��
	std::unordered_map<int, std::unordered_map<int, std::function<void(std::vector<float>& tmp, FILE* file)>>>load;
};
