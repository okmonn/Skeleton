#pragma once
#include "SndFunc.h"
#include <memory>
#include <thread>

struct IXAudio2SourceVoice;
struct XAUDIO2_VOICE_STATE;
class VoiceCallback;
class Filter;

class Sound
{
public:
	// �R���X�g���N�^
	Sound();
	Sound(const std::string& fileName);
	Sound(const snd::Info& info);
	
	// �f�X�g���N�^
	~Sound();

	// �ǂݍ���
	void Load(const std::string& fileName);

	// �T�E���h��񂩂�T�E���h�̐���
	void CopyInfo(const snd::Info& info);

	// �Đ�
	long Play(const bool& loop);

	// ��~
	long Stop(void);

	// �Đ��|�C���g�̃��Z�b�g
	void Reset(void);

private:
	//�R�s�[�R���X�g���N�^
	Sound(const Sound&) = delete;
	void operator=(const Sound&) = delete;

	// �\�[�X�{�C�X�̐���
	long CreateVoice(const std::string& fileName);
	long CreateVoice(const snd::Info& info);

	// �t�@�C������̔񓯊�����
	void StreamFile(void);

	// �T�E���h��񂩂�̔񓯊�����
	void StreamInfo(void);


	// �{�C�X�R�[���o�b�N
	std::unique_ptr<VoiceCallback>call;

	// �t�B���^�[
	std::unique_ptr<Filter>filter;

	// �{�C�X�X�e�[�^�X
	std::unique_ptr<XAUDIO2_VOICE_STATE>state;

	// �\�[�X�{�C�X
	IXAudio2SourceVoice* voice;

	// ���[�v�t���O
	bool loop;

	// �X���b�h�t���O
	bool flag;

	// �ǂݍ��݃C���f�b�N�X
	unsigned int read;

	// �ǂݍ��݃t�@�C����
	std::string name;

	// �T�E���h���
	snd::Snd sound;

	// �X���b�h
	std::thread th;
};
