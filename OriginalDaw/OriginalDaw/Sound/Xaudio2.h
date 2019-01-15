#pragma once

struct IXAudio2;

class Xaudio2
{
public:
	// �f�X�g���N�^
	~Xaudio2();

	// �C���X�^���X�ϐ��̎擾
	static Xaudio2& Get(void) {
		static Xaudio2 instance;
		return instance;
	}

	// Xaudio2�̎擾
	IXAudio2* GetAudio(void) const {
		return audio;
	}

private:
	// �R���X�g���N�^
	Xaudio2();
	Xaudio2(const Xaudio2&) = delete;
	void operator=(const Xaudio2&) = delete;

	// COM�̏�����
	long InitCom(void);

	// Xaudio2�̐���
	long Create(void);


	// Xaudio2
	IXAudio2* audio;
};
