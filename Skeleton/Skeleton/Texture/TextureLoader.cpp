#include "TextureLoader.h"
#include "WICTextureLoader12.h"
#include "../Device/Device.h"
#include "../etc/Release.h"
#include "../etc/Func.h"

// �R���X�g���N�^
TextureLoader::TextureLoader()
{
	origin.clear();
}

// �f�X�g���N�^
TextureLoader::~TextureLoader()
{
	for (auto itr = origin.begin(); itr != origin.end(); ++itr)
	{
		Release(itr->second.rsc);
		itr->second.decode.release();
	}
}

// �ǂݍ���
long TextureLoader::Load(std::weak_ptr<Device>dev, const std::string & fileName)
{
	if (origin.find(fileName) != origin.end())
	{
		return S_OK;
	}

	auto path = func::ChangeWString(fileName);
	origin[fileName].sub = std::make_shared<D3D12_SUBRESOURCE_DATA>();

	auto hr = DirectX::LoadWICTextureFromFile(dev.lock()->Get(), path.c_str(), &origin[fileName].rsc, origin[fileName].decode, (*origin[fileName].sub));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�e�N�X�`���̓ǂݍ��݁F���s\n"));
	}

	return hr;
}

// �摜�̉����̎擾
unsigned int TextureLoader::GetWidth(const std::string & fileName)
{
	return static_cast<unsigned int>(origin[fileName].rsc->GetDesc().Width);
}

// �摜�̉����̎擾
unsigned int TextureLoader::GetHeight(const std::string & fileName)
{
	return origin[fileName].rsc->GetDesc().Height;
}

// �f�[�^�̍폜
void TextureLoader::Delete(const std::string & fileName)
{
	if (origin.find(fileName) != origin.end())
	{
		Release(origin[fileName].rsc);
		origin[fileName].decode.release();
		origin.erase(origin.find(fileName));
	}
}
