#include "RootMane.h"
#include "Root.h"
#include "../Useful/Useful.h"
#include "../Release.h"

#pragma comment(lib, "d3dcompiler.lib")

// �R���X�g���N�^
RootMane::RootMane()
{
	root.clear();
}

// �f�X�g���N�^
RootMane::~RootMane()
{
}

// ���[�g�̐���
void RootMane::Create(const std::string & name, const std::string & fileName)
{
	if (root.find(name) != root.end())
	{
		OutputDebugString(_T("\n���[�g�����ς�\n"));
		return;
	}

	root[name] = std::make_shared<Root>(use::ChangeCode(fileName));
}

// ���[�g�̎擾
std::shared_ptr<Root> RootMane::GetRoot(const std::string & name)
{
	if (root.find(name) == root.end())
	{
		return nullptr;
	}

	return root[name];
}

// ���[�g�̍폜
void RootMane::Delete(const std::string & name)
{
	if (root.find(name) != root.end())
	{
		root.erase(root.find(name));
	}
}
