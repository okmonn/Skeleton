#include "SndLoader.h"
#include <Windows.h>
#include <tchar.h>

// �X���b�h��
#define THREAD_MAX 5

// �I�t�Z�b�g
#define OFFSET 10

// �R���X�g���N�^
SndLoader::SndLoader() : 
	flag(true)
{
	sound.clear();
	th.resize(THREAD_MAX);

	Create();
}

// �f�X�g���N�^
SndLoader::~SndLoader()
{
	flag = false;
	for (auto& i : th)
	{
		if (i.joinable() == true)
		{
			i.join();
		}
	}
}

// �ǂݍ��݃e�[�u���̍쐬
void SndLoader::Create(void)
{
	load[1][8] = [&](std::vector<float>& tmp, FILE* file)->void {
		snd::LoadMono8(tmp, file);
	};

	load[1][16] = [&](std::vector<float>& tmp, FILE* file)->void {
		snd::LoadMono16(tmp, file);
	};

	load[2][8] = [&](std::vector<float>& tmp, FILE* file)->void {
		snd::LoadStereo8(tmp, file);
	};

	load[2][16] = [&](std::vector<float>& tmp, FILE* file)->void {
		snd::LoadStereo16(tmp, file);
	};
}

// �ǂݍ���
int SndLoader::Load(const std::string & fileName)
{
	if (sound.find(fileName) != sound.end())
	{
		OutputDebugString(_T("\n�ǂݍ��ݍς�\n"));
		return -1;
	}

	if (fopen_s(&sound[fileName].file, fileName.c_str(), "rb") != 0)
	{
		OutputDebugString(_T("\n�t�@�C���̎Q�ƁF���s\n"));
		return -1;
	}

	snd::RIFF riff{};
	snd::FMT fmt{};
	snd::DATA data{};

	snd::LoadRIFF(riff, sound[fileName].file);
	snd::LoadFMT(fmt, sound[fileName].file);
	snd::LoadDATA(data, sound[fileName].file);

	//���l�̊i�[
	sound[fileName].channel = fmt.channel;
	sound[fileName].sample  = fmt.sample;
	sound[fileName].bit     = fmt.bit;
	sound[fileName].length  = data.size / fmt.byte / fmt.channel;

	// �ʃX���b�h�œǂݍ���
	bool flag = true;
	while (flag)
	{
		for (auto& i : th)
		{
			if (i.joinable() == false)
			{
				sound[fileName].data.resize((size_t)std::ceilf((float)data.size / (float)(fmt.sample / OFFSET) / (float)fmt.channel));
				i = std::thread(&SndLoader::Stream, this, fileName);
				flag = false;
				break;
			}
		}
	}
}

// �񓯊��ǂݍ���
void SndLoader::Stream(const std::string & fileName)
{
	std::lock_guard<std::mutex>lock(mtx);

	int read = 0;

	while (std::feof(sound[fileName].file) == 0 && flag == true)
	{
		sound[fileName].data[read].resize(sound[fileName].sample / 10);
		load[sound[fileName].channel][sound[fileName].bit](sound[fileName].data[read], sound[fileName].file);
		++read;
	}

	fclose(sound[fileName].file);
}

// �폜
void SndLoader::Delete(const std::string & fileName)
{
	if (sound.find(fileName) != sound.end())
	{
		sound.erase(sound.find(fileName));
	}
}
