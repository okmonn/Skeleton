#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

class Queue;
class List;
class Fence;

class Compute
{
	// ���
	struct Info {
		//���\�[�XID
		int rsc;
		//���M�f�[�^
		void* data;
	};
public:
	// �R���X�g���N�^
	Compute();
	// �f�X�g���N�^
	virtual ~Compute();

	// �f�[�^�̃R�s�[
	template<typename T>
	void Copy(const std::string& name, const T& data);

protected:
	// �V�F�[�_�̓ǂݍ���
	void Load(const std::string& name, const std::wstring& fileName);

	// �}�b�v
	long Map(const std::string& name);

	// CBV�̐���
	void CBV(const std::string& name, const size_t& size);

	// UAV�̐���
	void UAV(const std::string& name, const size_t& stride, const size_t& num);

	// �A���}�b�v
	void Unmap(const std::string& name);

	// ���Z�b�g
	void Reset(const std::string& name);

	// �f�[�^�̃R�s�[
	template<typename T>
	void Copy(const std::string& name, const std::vector<T>& data);

	// �q�[�v�̃Z�b�g
	void SetHeap(void);

	// ���\�[�X�̃Z�b�g
	void SetRsc(void);

	// �f�[�^�̍X�V
	void UpData(const std::string& name, std::vector<float>& data);

	// �I��
	void End(void);


	// �L���[
	std::shared_ptr<Queue>queue;

	// ���X�g
	std::unique_ptr<List>list;

	// �t�F���X
	std::unique_ptr<Fence>fence;

	// �q�[�vID
	int heap;

	// ���\�[�X���
	std::unordered_map<std::string, Info>info;
};

// �f�[�^�̃R�s�[
template<typename T>
inline void Compute::Copy(const std::string & name, const T & data)
{
	if (info.find(name) == info.end())
	{
		return;
	}

	memcpy(info[name].data, &data, sizeof(T));
}

// �f�[�^�̃R�s�[
template<typename T>
inline void Compute::Copy(const std::string & name, const std::vector<T>& data)
{
	if (info.find(name) == info.end())
	{
		return;
	}

	memcpy(info[name].data, data.data(), sizeof(T) * data.size());
}
