#pragma once
#include "PmdData.h"
#include <string>
#include <memory>
#include <unordered_map>

class PmdLoader;
class TextureLoader;
class DescriptorMane;
class Device;
class List;
class Camera;
class Light;
class Root;
class Pipe;

class Pmd
{
	struct WVP {
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
		DirectX::XMFLOAT3 eyePos;
		DirectX::XMFLOAT3 lightPos;
	};

	// �f�[�^
	struct Data {
		//�o���h���p���X�g
		std::unique_ptr<List>list;

		//WVP�p���\�[�X
		int cRsc;
		WVP* wvp;

		//�}�e���A���p���\�[�X
		int mRsc;
		pmd::Mat mat;
		unsigned __int8* materialData;
	};

public:
	// �R���X�g���N�^
	Pmd(std::weak_ptr<Device>dev, std::weak_ptr<Camera>cam, std::weak_ptr<Light>light, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// �f�X�g���N�^
	~Pmd();

	// �ǂݍ���
	void Load(const std::string& fileName, int& i);

	// ��]
	void Rotate(int& i, const float& angle);
	
	// �`��
	void Draw(std::weak_ptr<List>list, int& i);

private:
	// �V�F�[�_�[�r���[�̐���
	void CreateShaderView(const std::string& fileName, int* i, const int& index = 0);

	// �T�u���\�[�X�ɏ�������
	long WriteSub(const std::string& fileName);

	// �萔���\�[�X�̐���
	long CreateConRsc(int* i, int& rsc, const unsigned __int64& size);

	// �萔�r���[�̐���
	void CreateConView(int* i, int& rsc, const unsigned int& size, const int& index = 0);

	// �}�b�v
	long Map(int& rsc, void** data);

	// �o���h���̃Z�b�g
	void Bundle(const std::string& fileName, int* i);


	// PMD���[�_�[
	PmdLoader& loader;

	// �e�N�X�`�����[�_�[
	TextureLoader& tex;

	// �f�B�X�N���v�^�[�}�l�[�W���[
	DescriptorMane& descMane;

	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �J����
	std::weak_ptr<Camera>cam;

	// ���C�g
	std::weak_ptr<Light>light;

	// ���[�g
	std::weak_ptr<Root>root;

	// �p�C�v
	std::weak_ptr<Pipe>pipe;

	// ���\�[�X�쐬�ԍ�
	int index;

	// Pmd�f�[�^
	std::unordered_map<int*, Data>data;
};
