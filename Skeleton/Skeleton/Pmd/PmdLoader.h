#pragma once
#include "PmdData.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

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
		//���_���M�f�[�^
		void* vertexData;
		//�C���f�b�N�X���\�[�X
		int iRsc;
		void* indexData;
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

private:
	// �R���X�g���N�^
	PmdLoader();
	PmdLoader(const PmdLoader&)      = delete;
	void operator=(const PmdLoader&) = delete;

	// ���_���\�[�X�̐���
	long CreateRsc(std::weak_ptr<Device>dev, int* i, const unsigned long long& size);

	// �}�b�v
	template<typename T>
	long Map(int* i, T* data, const unsigned int& size, void** rscData);


	// �f�B�X�N���v�^�[�}�l�[�W���[
	DescriptorMane& descMane;

	// �ǂݍ��݃f�[�^
	std::unordered_map<std::string, Data>data;
};
