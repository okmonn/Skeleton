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
		std::shared_ptr<std::vector<pmd::Vertex>>vertex;
		//�C���f�b�N�X���
		std::shared_ptr<std::vector<unsigned short>>index;
		//�}�e���A�����
		std::shared_ptr<std::vector<pmd::Material>>material;
		//�{�[�����
		std::shared_ptr<std::vector<pmd::Born>>born;

		//���_���\�[�X
		int vRsc;
		//�C���f�b�N�X���\�[�X
		int iRsc;
		//�}�e���A���p���\�[�X
		int mRsc;
		//�{�[���p���\�[�X
		int bRsc;
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

private:
	// �R���X�g���N�^
	PmdLoader();
	PmdLoader(const PmdLoader&)      = delete;
	void operator=(const PmdLoader&) = delete;

	// ���_���\�[�X�̐���
	long CreateVertexRsc(std::weak_ptr<Device>dev, const std::string& fileName);


	// �f�B�X�N���v�^�[�}�l�[�W���[
	DescriptorMane& descMane;

	// �ǂݍ��݃f�[�^
	std::unordered_map<std::string, Data>data;
};
