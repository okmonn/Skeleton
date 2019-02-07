#pragma once
#include "SndFunc.h"
#include <memory>
#include <thread>

struct IXAudio2SourceVoice;
struct XAUDIO2_VOICE_STATE;
class VoiceCallback;
class Effector;
class Delay;
class Filter;

class Sound
{
public:
	// �R���X�g���N�^
	Sound();
	Sound(const std::string& fileName);
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

	// �p�����[�^�̎擾
	snd::Param GetParam(void) const;
	// �p�����[�^�̃Z�b�g
	void SetParam(const snd::Param& param);

	// �f�B���C�p�����[�^�̎擾
	snd::DelayParam GetDelay(void) const;
	// �f�B���C�p�����[�^�̃Z�b�g
	void SetDelay(const snd::DelayParam& param);

	// �t�B���^�W���̃��Z�b�g
	void FilterReset(void);

	// ���[�p�X
	void LowPass(const float& cut, const float& q = 1.0f / std::sqrt(2.0f));

	// �n�C�p�X
	void HighPass(const float& cut, const float& q = 1.0f / std::sqrt(2.0f));

	// �o���h�p�X
	void BandPass(const float& cut, const float& bw = 1.0f);

	// �T�E���h���̎擾
	snd::Info Getinfo(void) const {
		return info;
	}
	// ���ݔg�`�̎擾
	std::vector<float> GetData(void) const {
		return data[index];
	}

private:
	// �\�[�X�{�C�X�̐���
	long CreateVoice(void);

	// �񓯊�����
	void Stream(void);


	// �R�[���o�b�N
	std::unique_ptr<VoiceCallback>back;

	// �G�t�F�N�^�[
	std::unique_ptr<Effector>effe;

	// �f�B���C
	std::unique_ptr<Delay>delay;

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

	// �T�E���h�o�b�t�@
	std::vector<std::vector<float>>data;

	// �X���b�h
	std::thread th;
};
