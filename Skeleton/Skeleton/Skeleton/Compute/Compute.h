#pragma once
#include <string>
#include <memory>
#include <unordered_map>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class Queue;
class List;
class Fence;

class Compute
{
public:
	// �R���X�g���N�^
	Compute();
	// �f�X�g���N�^
	virtual ~Compute();

private:
	// �萔���\�[�X�̐���
	long CreateCRsc(const std::string& name, const size_t& size);

	// �񏇏����\�[�X�̐���
	long CreateURsc(const std::string& name, const size_t& stride, const size_t& num);

protected:
	// �q�[�v�̐���
	long CreateHeap(const unsigned int& num);

	// CBV�̐���
	void CBV(const std::string& name, const size_t& size);

	// UAV�̐���
	void UAV(const std::string& name, const size_t& stride, const size_t& num);

	// �}�b�v
	long Map(const std::string& name);

	// �A���}�b�v
	void Unmap(const std::string& name);

	// �f�[�^�̃��Z�b�g
	void Reset(const std::string& name);

	// �I��
	void End(void);


	// �L���[
	std::shared_ptr<Queue>queue;

	// ���X�g
	std::unique_ptr<List>list;

	// �t�F���X
	std::unique_ptr<Fence>fence;

	// �q�[�v
	ID3D12DescriptorHeap* heap;

	// �C���f�b�N�X
	unsigned int index;

	// ���\�[�X
	std::unordered_map<std::string, ID3D12Resource*>rsc;

	// ����M�p�f�[�^
	std::unordered_map<std::string, void*>data;
};
