#pragma once
#include "SndFunc.h"
#include <memory>
#include <thread>

struct IXAudio2SourceVoice;
struct XAUDIO2_VOICE_STATE;
class VoiceCallback;
class Effector;
class Shifter;
class Filter;

class Sound
{
public:
	// �R���X�g���N�^
	Sound();
	// �f�X�g���N�^
	~Sound();

	// �ǂݍ���
	int Load(const std::string& fileName);

	// �Đ�
	long Play(const bool& loop = false);

	// ��~
	long Stop(void);

	// ���[�v�ʒu�̃Z�b�g
	void SetLoopPos(void);

	// ���[�v�ʒu�Ɉړ�
	void MoveLoopPos(void);

private:
	// �\�[�X�{�C�X�̐���
	long CreateVoice(void);

	// �񓯊�����
	void Stream(void);


	// �R�[���o�b�N
	std::unique_ptr<VoiceCallback>back;

	// �G�t�F�N�^�[
	std::unique_ptr<Effector>effe;

	// �V�t�^�[
	std::unique_ptr<Shifter>shifter;

	// �t�B���^�[
	std::unique_ptr<Filter>filter;

	// �{�C�X�X�e�[�^�X
	std::unique_ptr<XAUDIO2_VOICE_STATE>st;

	// �\�[�X�{�C�X
	IXAudio2SourceVoice* voice;

	// �C���f�b�N�X
	unsigned int index;

	// �ǂݍ��݃C���f�b�N�X
	unsigned int read;

	// ���[�v�|�C���g
	unsigned int loopPos;

	// ���[�v�t���O
	bool loop;

	// �X���b�h�t���O
	bool threadFlag;

	// �ǂݍ��݃t�@�C����
	std::string name;

	// �T�E���h���
	snd::Info info;

	// �p�����[�^
	snd::Param param;

	// �T�E���h�o�b�t�@
	std::vector<std::vector<float>>data;

	// �X���b�h
	std::thread th;
};
