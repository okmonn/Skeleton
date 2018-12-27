#pragma once
#include <string>
#include <vector>

namespace mp3 
{
#pragma pack(1)
	// ID3v2�w�b�_�[
	struct ID3v2Header {
		//���ʎq
		unsigned char id[3];
		//�o�[�W����
		short ver;
		//�t���O
		unsigned char flag;
		//�T�C�Y
		long size;
	};
#pragma pack()
	// ID3v2�t���[��
	struct ID3v2Fream {
		//�t���[��ID
		unsigned char id[4];
		//�T�C�Y
		long size;
		//�t���O
		short flag;
		//���e
		std::vector<char>contents;
	};
	
	// �w�b�_�[�̓ǂݍ���
	long LoadHeader(ID3v2Header& header, FILE* file);

	// �t���[���̓ǂݍ���
	void LoadFream(std::vector<ID3v2Fream>& freams, FILE* file);

	// MPEG�t���[���̓ǂݍ���
	void LoadMpeg(std::vector<unsigned char>& data, FILE* file);
	
	// �ǂݍ���
	int Load(const std::string& fileName);
}
