#pragma once
#include "PmdData.h"
#include <map>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <unordered_map>

class TextureLoader;
class DescriptorMane;
class Device;

class PmdLoader
{
	// �ǂݍ��݃f�[�^
	struct Data {
		//���_���
		std::vector<pmd::Vertex>vertex;
		//�C���f�b�N�X���
		std::vector<unsigned short>index;
		//�}�e���A�����
		std::vector<pmd::Material>material;
		//�{�[�����
		std::vector<pmd::Born>born;

		//���_���\�[�X
		int vRsc;
		void* vertexData;
		//�C���f�b�N�X���\�[�X
		int iRsc;
		void* indexData;

		//�{�[���m�[�h
		std::map<std::string, pmd::BornNode>node;

		//�e�N�X�`��ID
		std::unordered_map<int, std::string>tex;
		//���Z�e�N�X�`��ID
		std::unordered_map<int, std::string>spa;
		//��Z�e�N�X�`��ID
		std::unordered_map<int, std::string>sph;
		//�g�D�[���e�N�X�`��ID
		std::unordered_map<int, std::string>toon;
	};

public:
	// �f�X�g���N�^
	~PmdLoader();

	// �C���X�^���X�ϐ��̎擾
	static PmdLoader& Get(void) {
		static PmdLoader instance;
		return instance;
	}

	// �ǂݍ���
	int Load(std::weak_ptr<Device>dev, const std::string& fileName);

	// ���_���̎擾
	std::vector<pmd::Vertex> GetVertex(const std::string& fileName) {
		return data[fileName].vertex;
	}
	// �C���f�b�N�X���̎擾
	std::vector<unsigned short> GetIndex(const std::string& fileName) {
		return data[fileName].index;
	}
	// �}�e���A�����̎擾
	std::vector<pmd::Material> GetMaterial(const std::string& fileName) {
		return data[fileName].material;
	}
	// �{�[�����̎擾
	std::vector<pmd::Born> GetBorn(const std::string& fileName) {
		return data[fileName].born;
	}
	// ���_���\�[�XID�̎擾
	int& GetVertexRsc(const std::string& fileName) {
		return data[fileName].vRsc;
	}
	// �C���f�b�N�X���\�[�XID�̎擾
	int& GetIndexRsc(const std::string& fileName) {
		return data[fileName].iRsc;
	}
	// �{�[���m�[�h�̎擾
	std::map<std::string, pmd::BornNode>& GetBornNode(const std::string& fileName) {
		return data[fileName].node;
	}
	// �e�N�X�`��ID�̎擾
	std::unordered_map<int, std::string>& GetTexture(const std::string& fileName) {
		return data[fileName].tex;
	}
	// ���Z�e�N�X�`��ID�̎擾
	std::unordered_map<int, std::string>& GetSpa(const std::string& fileName) {
		return data[fileName].spa;
	}
	// ��Z�e�N�X�`��ID�̎擾
	std::unordered_map<int, std::string>& GetSph(const std::string& fileName) {
		return data[fileName].sph;
	}
	// �g�D�[���e�N�X�`��ID�̎擾
	std::unordered_map<int, std::string>& GetToon(const std::string& fileName) {
		return data[fileName].toon;
	}

private:
	// �R���X�g���N�^
	PmdLoader();
	PmdLoader(const PmdLoader&)      = delete;
	void operator=(const PmdLoader&) = delete;

	// �e�N�X�`���̓ǂݍ���
	void LoadTex(std::weak_ptr<Device>dev, const std::string& fileName);

	// ���Z�e�N�X�`���̓ǂݍ���
	void LoadSpa(std::weak_ptr<Device>dev, const std::string& fileName, const std::string& pass, const unsigned int& index);

	// ��Z�e�N�X�`���̓ǂݍ���
	void LoadSph(std::weak_ptr<Device>dev, const std::string& fileName, const std::string& pass, const unsigned int& index);

	// �g�D�[���e�N�X�`���̓ǂݍ���
	void LoadToon(std::weak_ptr<Device>dev, const std::string& fileName);

	// ���_���\�[�X�̐���
	long CreateRsc(std::weak_ptr<Device>dev, int* i, const unsigned long long& size);

	// �}�b�v
	template<typename T>
	long Map(int* i, T* data, const unsigned int& size, void** rscData);

	// �{�[���m�[�h�̃Z�b�g
	void SetBornNode(const std::string& fileName);


	// �e�N�X�`�����[�_�[
	TextureLoader& tex;

	// �f�B�X�N���v�^�[�}�l�[�W���[
	DescriptorMane& descMane;

	// �g�D�[���e�N�X�`����
	std::array<char[100], 10>toonName;

	// �ǂݍ��݃f�[�^
	std::unordered_map<std::string, Data>data;
};
