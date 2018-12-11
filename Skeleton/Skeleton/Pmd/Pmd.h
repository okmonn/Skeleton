#pragma once
#include "PmdData.h"
#include "../Vmd/VmdData.h"
#include <map>
#include <string>
#include <memory>
#include <unordered_map>

class PmdLoader;
class TextureLoader;
class Vmd;
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
		DirectX::XMFLOAT4X4 lightView;
		DirectX::XMFLOAT4X4 lightProjection;
		DirectX::XMFLOAT3 eyePos;
		DirectX::XMFLOAT3 lightPos;
	};

	// �f�[�^
	struct Data {
		//���f���t�@�C����
		std::string model;

		//�o���h���p���X�g
		std::unique_ptr<List>list;
		std::unique_ptr<List>sList;

		//WVP�p���\�[�X
		int cRsc;
		WVP* wvp;

		//�{�[���p���\�[�X
		int bRsc;
		std::vector<DirectX::XMMATRIX>mtx;
		void* bornData;

		//�}�e���A���p���\�[�X
		int mRsc;
		pmd::Mat mat;
		unsigned __int8* materialData;

		//���[�V����
		std::weak_ptr<std::map<std::string, std::vector<vmd::Motion>>>motion;
		float flam;
		float animTime;
	};

public:
	// �R���X�g���N�^
	Pmd(std::weak_ptr<Device>dev, std::weak_ptr<Camera>cam, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe,
		std::weak_ptr<Root>sRoot, std::weak_ptr<Pipe>sPipe);
	// �f�X�g���N�^
	~Pmd();

	// �ǂݍ���
	void Load(const std::string& fileName, int& i);

	// ���[�V�����̓K��
	void Attach(const std::string& fileName, int& i);

	// ��]
	void Rotate(int& i, const float& angle);
	
	// �`��
	void Draw(std::weak_ptr<List>list, int& i);

	// �e�`��
	void DrawShadow(std::weak_ptr<List>list, int& i);

	// �A�j���[�V����
	void Animation(int& i, const bool& loop, const float& animSpeed);

	// �A�j���[�V�����I���m�F
	bool CheckAnimEnd(int& i);

	// ���f���̍폜
	void Delete(int& i);

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

	// �e�p�o���h���̃Z�b�g
	void ShadowBundle(const std::string& fileName, int* i);

	// �{�[���̉�]
	void RotateBorn(int* i, const std::string& name, const DirectX::XMMATRIX& mtx);

	// �{�[���̍ċA����
	void RecursiveBorn(int* i, pmd::BornNode& node, const DirectX::XMMATRIX& mtx);

	// �A�j���[�V�����̃��Z�b�g
	void ResetAnim(int* i);


	// PMD���[�_�[
	PmdLoader& loader;

	// �e�N�X�`�����[�_�[
	TextureLoader& tex;

	// VMD
	Vmd& motion;

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

	// �e�p���[�g
	std::weak_ptr<Root>sRoot;

	// �e�p�p�C�v
	std::weak_ptr<Pipe>sPipe;

	// ���\�[�X�쐬�ԍ�
	int index;

	// Pmd�f�[�^
	std::unordered_map<int*, Data>data;
};
