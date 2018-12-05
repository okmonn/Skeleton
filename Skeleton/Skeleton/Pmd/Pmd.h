#pragma once
#include <string>
#include <memory>
#include <unordered_map>

class Device;
class List;
class Root;
class Pipe;

class Pmd
{
public:
	// �R���X�g���N�^
	Pmd(std::weak_ptr<Device>dev, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// �f�X�g���N�^
	~Pmd();

	// �ǂݍ���
	void Load(const std::string& fileName, int& i);

private:
	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �o���h���p���X�g
	std::unique_ptr<List>list;

	// ���[�g
	std::weak_ptr<Root>root;

	// �p�C�v
	std::weak_ptr<Pipe>pipe;

	// Pmd�f�[�^

};
