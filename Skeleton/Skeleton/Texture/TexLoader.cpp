#include "TexLoader.h"
#include "WICTextureLoader12.h"
#include "../Useful/Useful.h"
#include "../Release.h"

// �R���X�g���N�^
TexLoader::TexLoader()
{
	info.clear();
}

// �f�X�g���N�^
TexLoader::~TexLoader()
{
	for (auto itr = info.begin(); itr != info.end(); ++itr)
	{
		Release(itr->second.rsc);
		itr->second.decode.release();
	}
}

// �ǂݍ���
long TexLoader::Load(const std::string & fileName)
{
	if (info.find(fileName) != info.end())
	{
		OutputDebugString(_T("\n�摜�ǂݍ��ݍς�\n"));
		return S_OK;
	}

	auto pass = use::ChangeCode(fileName);
	info[fileName].sub = std::make_shared<D3D12_SUBRESOURCE_DATA>();

	auto hr = DirectX::LoadWICTextureFromFile(Device::Get().GetDev(), pass.c_str(), &info[fileName].rsc, info[fileName].decode, *info[fileName].sub);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�e�N�X�`���̓ǂݍ��݁F���s\n"));
	}

	return hr;
}

// ���\�[�X�̎擾
ID3D12Resource * TexLoader::GetRsc(const std::string & fileName)
{
	if (info.find(fileName) == info.end())
	{
		return nullptr;
	}

	return info[fileName].rsc;
}

// �f�R�[�h�̎擾
unsigned char * TexLoader::GetDecode(const std::string & fileName)
{
	if (info.find(fileName) == info.end())
	{
		return nullptr;
	}

	return info[fileName].decode.get();
}

// �T�u�f�[�^�̎擾
std::shared_ptr<D3D12_SUBRESOURCE_DATA> TexLoader::GetSub(const std::string & fileName)
{
	if (info.find(fileName) == info.end())
	{
		return nullptr;
	}

	return info[fileName].sub;
}

// �摜�T�C�Y�̎擾
Vec2f TexLoader::GetSize(const std::string & fileName)
{
	if (info.find(fileName) == info.end())
	{
		return 0.0f;
	}

	return {static_cast<float>(info[fileName].rsc->GetDesc().Width), static_cast<float>(info[fileName].rsc->GetDesc().Height)};
}

// �摜�̍폜
void TexLoader::Delete(const std::string & fileName)
{
	if (info.find(fileName) != info.end())
	{
		info.erase(info.find(fileName));
	}
}
