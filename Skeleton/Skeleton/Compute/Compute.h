#pragma once
#include "../etc/tString.h"
#include <map>
#include <memory>

struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;
struct ID3D12RootSignature;
struct ID3D12PipelineState;
struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class DescriptorMane;
class Device;
class Queue;
class List;
class Fence;

class Compute
{
	struct Info {
		//���\�[�XID
		int rsc;
		//����M�f�[�^
		void* data;
	};

	// �R���X�g���N�^
	Compute();
	// �f�X�g���N�^
	virtual ~Compute();

protected:
	// �N���X�̃C���X�^���X
	void Create(void);

	// ���[�g�V�O�l�`���̐���
	long CreateRoot(const std::tstring& fileName);

	// �p�C�v���C���̐���
	long CreatePipe(void);

	// UAV���\�[�X�̐���
	long CreateUavRsc(const std::string& name, const unsigned int& size);

	// UAV�̐���
	void CreateUnorderView(const std::string& name, const unsigned int& stride, const unsigned int& num);

	// �}�b�v
	long Map(const std::string& name);

	// �A���}�b�v
	void UnMap(const std::string& name);

	// UAV�̐���
	void UAV(const std::string& name, const unsigned int& stride, const unsigned int& num);


	// �f�B�X�N���v�^�}�l�[�W���[
	DescriptorMane& descMane;

	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �L���[
	std::shared_ptr<Queue>queue;

	// ���X�g
	std::unique_ptr<List>list;

	// �t�F���X
	std::unique_ptr<Fence>fence;

	// �R���s���[�g�V�F�[�_�[���
	ID3DBlob* shader;

	// �R���s���[�g�p���[�g�V�O�l�`��
	ID3D12RootSignature* root;

	// �R���s���[�g�p�p�C�v���C��
	ID3D12PipelineState* pipe;

	// �q�[�vID
	int heap;

	// ���\�[�X���
	std::map<std::string, Info>info;
};
