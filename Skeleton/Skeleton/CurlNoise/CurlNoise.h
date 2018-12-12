#pragma once
#include "../etc/tString.h"
#include <map>
#include <memory>
#include <vector>
#include <DirectXMath.h>

struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;
struct ID3D12RootSignature;
struct ID3D12PipelineState;
struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class Device;
class Queue;
class List;
class Fence;

class CurlNoise
{
	struct Info {
		//���\�[�X
		ID3D12Resource* rsc;
		//���M�f�[�^
		DirectX::XMFLOAT3* data;
		//�ԍ�
		int index;
	};

public:
	// �R���X�g���N�^
	CurlNoise(std::weak_ptr<Device>dev, const std::tstring& fileName);
	// �f�X�g���N�^
	~CurlNoise();

	// UAV�̐���
	void UAV(const std::string& name, const unsigned int& stride, const unsigned int& num);

	// �f�[�^�̏�����
	void Init(const std::vector<DirectX::XMFLOAT3>& pos);

	// ���s
	void Execution(void);

private:
	// ���[�g�V�O�l�`���̐���
	long CreateRoot(const std::tstring& fileName);

	// �p�C�v���C���̐���
	long CreatePipe(void);

	// �q�[�v�̐���
	long CreateHeap(void);

	// UAV���\�[�X�̐���
	long CreateUavRsc(const std::string& name, const unsigned int& size);

	// UAV�̐���
	void CreateUnorderView(const std::string& name, const unsigned int& stride, const unsigned int& num);

	// �}�b�v
	long Map(const std::string& name);


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

	// �R���s���[�g���[�g�V�O�l�`��
	ID3D12RootSignature* root;

	// �R���s���[�g�p�C�v���C��
	ID3D12PipelineState* pipe;

	// �q�[�v
	ID3D12DescriptorHeap* heap;

	// ���\�[�X�ԍ�
	int index;

	// �o�b�t�@
	std::map<std::string, Info>info;

	// �f�[�^
	std::vector<DirectX::XMFLOAT3>pos;
};
