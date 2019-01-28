#pragma once
#include "SndFunc.h"
#include <memory>
#include <thread>

struct IXAudio2SourceVoice;
class VoiceCallback;

class Sound
{
public:
	// �R���X�g���N�^
	Sound();
	// �f�X�g���N�^
	~Sound();

	// �ǂݍ���
	int Load(const std::string& fileName);

private:
	// �\�[�X�{�C�X�̐���
	long CreateVoice(void);


	// �R�[���o�b�N
	std::unique_ptr<VoiceCallback>back;

	// �\�[�X�{�C�X
	IXAudio2SourceVoice* voice;

	// �X���b�h�t���O
	bool threadFlag;

	// �ǂݍ��݃t�@�C����
	std::string name;

	// �T�E���h���
	snd::Info info;

	// �T�E���h�o�b�t�@
	std::vector<std::vector<float>>data;

	// �X���b�h
	std::thread th;
};
