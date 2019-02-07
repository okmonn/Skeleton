#pragma once
#include <string>
#include <memory>
#include <unordered_map>

class Root;

class RootMane
{
public:
	// �f�X�g���N�^
	~RootMane();

	// �C���X�^���X�ϐ��̎擾
	static RootMane& Get(void) {
		static RootMane instance;
		return instance;
	}

	// ���[�g�̐���
	void Create(const std::string& name, const std::string& fileName);

	// ���[�g�̎擾
	std::shared_ptr<Root> GetRoot(const std::string& name);

	// ���[�g�̍폜
	void Delete(const std::string& name);

private:
	// �R���X�g���N�^
	RootMane();
	RootMane(const RootMane&) = delete;
	void operator=(const RootMane&) = delete;


	// ���[�g
	std::unordered_map<std::string, std::shared_ptr<Root>>root;
};
