#pragma once
#include "PmdData.h"
#include <string>
#include <memory>
#include <unordered_map>

class PmdLoader;
class DescriptorMane;
class Device;
class List;
class Camera;
class Root;
class Pipe;

class Pmd
{
	struct WVP {
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

	// �f�[�^
	struct Data {
		//�o���h���p���X�g
		std::unique_ptr<List>list;
		//���_�f�[�^
		std::weak_ptr<std::vector<pmd::Vertex>>vertex;

		//WVP�p���\�[�X
		int cRsc;
		//WVP
		WVP* wvp;
	};

public:
	// �R���X�g���N�^
	Pmd(std::weak_ptr<Device>dev, std::weak_ptr<Camera>cam, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// �f�X�g���N�^
	~Pmd();

	// �ǂݍ���
	void Load(const std::string& fileName, int& i);

	// ��]
	void Rotate(int& i, const float& angle);
	
	// �`��
	void Draw(std::weak_ptr<List>list, int& i);

private:
	// �萔���\�[�X�̐���
	long CreateConRsc(int* i, int& rsc);

	// �萔�r���[�̐���
	void CreateConView(int* i, int& rsc);

	// �}�b�v
	long Map(int* i, int& rsc);

	// �o���h���̃Z�b�g
	void Bundle(const std::string& fileName, int* i);


	// PMD���[�_�[
	PmdLoader& loader;

	// �f�B�X�N���v�^�[�}�l�[�W���[
	DescriptorMane& descMane;

	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �J����
	std::weak_ptr<Camera>cam;

	// ���[�g
	std::weak_ptr<Root>root;

	// �p�C�v
	std::weak_ptr<Pipe>pipe;

	// Pmd�f�[�^
	std::unordered_map<int*, Data>data;
};
