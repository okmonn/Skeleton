#include "TexLoader.h"
#include "WICTextureLoader12.h"
#include "../Useful/Useful.h"
#include "../Release.h"

// コンストラクタ
TexLoader::TexLoader()
{
	info.clear();
}

// デストラクタ
TexLoader::~TexLoader()
{
	for (auto itr = info.begin(); itr != info.end(); ++itr)
	{
		Release(itr->second.rsc);
		itr->second.decode.release();
	}
}

// 読み込み
long TexLoader::Load(const std::string & fileName)
{
	if (info.find(fileName) != info.end())
	{
		OutputDebugString(_T("\n画像読み込み済み\n"));
		return S_OK;
	}

	auto pass = use::ChangeCode(fileName);
	info[fileName].sub = std::make_shared<D3D12_SUBRESOURCE_DATA>();

	auto hr = DirectX::LoadWICTextureFromFile(Device::Get().GetDev(), pass.c_str(), &info[fileName].rsc, info[fileName].decode, *info[fileName].sub);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nテクスチャの読み込み：失敗\n"));
	}

	return hr;
}

// リソースの取得
ID3D12Resource * TexLoader::GetRsc(const std::string & fileName)
{
	if (info.find(fileName) == info.end())
	{
		return nullptr;
	}

	return info[fileName].rsc;
}

// デコードの取得
unsigned char * TexLoader::GetDecode(const std::string & fileName)
{
	if (info.find(fileName) == info.end())
	{
		return nullptr;
	}

	return info[fileName].decode.get();
}

// サブデータの取得
std::shared_ptr<D3D12_SUBRESOURCE_DATA> TexLoader::GetSub(const std::string & fileName)
{
	if (info.find(fileName) == info.end())
	{
		return nullptr;
	}

	return info[fileName].sub;
}

// 画像サイズの取得
Vec2f TexLoader::GetSize(const std::string & fileName)
{
	if (info.find(fileName) == info.end())
	{
		return 0.0f;
	}

	return {static_cast<float>(info[fileName].rsc->GetDesc().Width), static_cast<float>(info[fileName].rsc->GetDesc().Height)};
}

// 画像の削除
void TexLoader::Delete(const std::string & fileName)
{
	if (info.find(fileName) != info.end())
	{
		info.erase(info.find(fileName));
	}
}
