#pragma once
#include "SndFunc.h"
#include <memory>
#include <thread>
#include <cmath>

struct IXAudio2SourceVoice;
struct XAUDIO2_VOICE_STATE;
class VoiceCallback;
class Filter;
class Effector;

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

	// �T�E���h���\���̂̃R�s�[
	void CopyInfo(const snd::Info& info);

	// ���[�p�X�t�B���^
	void LowPass(const float& cutoff, const float& sample, const float& q = 1.0f / std::sqrtf(2.0f));

	// �n�C�p�X�t�B���^
	void HightPass(const float& cutoff, const float& sample, const float& q = 1.0f / std::sqrtf(2.0f));

	// �o���h�p�X�t�B���^
	void BandPass(const float& cutoff, const float& sample, const float& bw = 1.0f);

	// �Đ�
	long Play(const bool& loop);

	// ��~
	long Stop(void);

	// ���[�v�J�n�ʒu�̃Z�b�g
	void SetLoopPos(void);

	// ���[�v�J�n�ʒu�Ɉړ�
	void MoveLoopPos(void);

	// ���݂̃T�E���h���̎擾
	std::vector<float> GetData(void) const {
		return data[index];
	}

private:
	//�R�s�[�R���X�g���N�^
	Sound(const Sound&) = delete;
	void operator=(const Sound&) = delete;

	// �\�[�X�{�C�X�̐���
	long CreateVoice(void);

	// �t�@�C������̔񓯊�����
	void StreamFile(void);

	// �R�s�[�f�[�^����񓯊�����
	void StreamInfo(void);


	// �{�C�X�R�[���o�b�N
	std::unique_ptr<VoiceCallback>call;

	// �t�B���^�[
	std::unique_ptr<Filter>filter;

	// �G�t�F�N�^�[
	std::unique_ptr<Effector>effe;

	// �{�C�X�X�e�[�^�X
	std::unique_ptr<XAUDIO2_VOICE_STATE>state;

	// �\�[�X�{�C�X
	IXAudio2SourceVoice* voice;

	// ���[�v�t���O
	bool loop;

	// �X���b�h�t���O
	bool flag;

	// �o�b�t�@�C���f�b�N�X
	unsigned int index;

	// �ǂݍ��݃C���f�b�N�X
	unsigned int read;

	// ���[�v�J�n�ʒu
	unsigned int loopPos;

	// �ǂݍ��݃t�@�C����
	std::string name;

	// �p�����[�^
	snd::Param param;
	
	// �R�s�[���
	snd::Info copy;

	// �g�`���
	std::vector<std::vector<float>>data;

	// �X���b�h
	std::thread th;
};
