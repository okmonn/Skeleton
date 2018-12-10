#include "PmdLoader.h"
#include "../Texture/TextureLoader.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../etc/Release.h"
#include "../etc/Func.h"
#include <iostream>

// �R���X�g���N�^
PmdLoader::PmdLoader() : 
	tex(TextureLoader::Get()), descMane(DescriptorMane::Get())
{
	data.clear();
}

// �f�X�g���N�^
PmdLoader::~PmdLoader()
{
	for (auto itr = data.begin(); itr != data.end(); ++itr)
	{
		descMane.DeleteRsc(itr->second.vRsc);
	}
}

// �e�N�X�`���̓ǂݍ���
void PmdLoader::LoadTex(std::weak_ptr<Device>dev, const std::string & fileName)
{
	for (unsigned int i = 0; i < data[fileName].material.size(); ++i)
	{
		if (data[fileName].material[i].texPath[0] == '\0')
		{
			continue;
		}

		std::string tmp = (char*)data[fileName].material[i].texPath;

		std::string pass;
		if (tmp.find("*") != std::string::npos)
		{
			auto tmp1 = tmp.substr(0, tmp.find_first_of('*'));
			pass = func::FindString(fileName, '/') + tmp1;
			tex.Load(dev, pass);
			data[fileName].tex.emplace(i, pass);

			tmp1 = tmp.substr(tmp.find_first_of('*') + 1, tmp.size());
			pass = func::FindString(fileName, '/') + tmp1;
			LoadSph(dev, fileName, pass, i);

			continue;
		}

		pass = func::FindString(fileName, '/') + tmp;
		if (pass.find(".spa") != std::string::npos)
		{
			LoadSpa(dev, fileName, pass, i);
			continue;
		}

		if (pass.find(".sph") != std::string::npos)
		{
			LoadSph(dev, fileName, pass, i);
			continue;
		}

		tex.Load(dev, pass);
		data[fileName].tex.emplace(i, pass);
	}
}

// ���Z�e�N�X�`���̓ǂݍ���
void PmdLoader::LoadSpa(std::weak_ptr<Device> dev, const std::string & fileName, const std::string & pass, const unsigned int & index)
{
	tex.Load(dev, pass);
	data[fileName].spa.emplace(index, pass);
}

// ��Z�e�N�X�`���̓ǂݍ���
void PmdLoader::LoadSph(std::weak_ptr<Device> dev, const std::string & fileName, const std::string & pass, const unsigned int & index)
{
	tex.Load(dev, pass);
	data[fileName].sph.emplace(index, pass);
}

// �g�D�[���e�N�X�`���̓ǂݍ���
void PmdLoader::LoadToon(std::weak_ptr<Device> dev, const std::string & fileName)
{
	for (unsigned int i = 0; i < toonName.size(); ++i)
	{
		std::string tmp = toonName[i];
		auto path = func::FindString(fileName, '/') + "toon/" + tmp;
		tex.Load(dev, path);
		data[fileName].toon.emplace(i, path);
	}
}

// ���_���\�[�X�̐���
long PmdLoader::CreateRsc(std::weak_ptr<Device>dev, int * i, const unsigned long long & size)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.VisibleNodeMask      = 1;

	D3D12_RESOURCE_DESC desc{};
	desc.Alignment        = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height           = 1;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels        = 1;
	desc.SampleDesc       = { 1, 0 };
	desc.Width            = size;

	return descMane.CreateRsc(dev, *i, prop, desc);
}

// �}�b�v
template<typename T>
long PmdLoader::Map(int * i, T * data, const unsigned int & size, void ** rscData)
{
	auto hr = descMane.GetRsc(*i)->Map(0, nullptr, rscData);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nPMD�̃}�b�v�F���s\n"));
		return hr;
	}

	memcpy(*rscData, data, sizeof(data[0]) * size);

	descMane.GetRsc(*i)->Unmap(0, nullptr);

	return hr;
}

// �{�[���m�[�h�̃Z�b�g
void PmdLoader::SetBornNode(const std::string & fileName)
{
	for (unsigned int i = 0; i < data[fileName].born.size(); ++i)
	{
		data[fileName].node[data[fileName].born[i].name].index = i;
		data[fileName].node[data[fileName].born[i].name].start = data[fileName].born[i].pos;
	}

	for (auto& n : data[fileName].node)
	{
		if (data[fileName].born[n.second.index].pIndex >= data[fileName].born.size())
		{
			continue;
		}

		auto name = data[fileName].born[data[fileName].born[n.second.index].pIndex].name;
		data[fileName].node[name].child.push_back(&n.second);
	}
}

// �ǂݍ���
int PmdLoader::Load(std::weak_ptr<Device>dev, const std::string & fileName)
{
	if (data.find(fileName) != data.end())
	{
		return 0;
	}

	FILE* file = nullptr;
	if (fopen_s(&file, fileName.c_str(), "rb") != 0)
	{
		OutputDebugString(_T("\nPMD�̃t�@�C���Q�ƁF���s\n"));
		return -1;
	}

	//�w�b�_�[�ǂݍ���
	pmd::Header header = {};
	fread(&header, sizeof(header), 1, file);

	unsigned int num = 0;

	//���_�̓ǂݍ���
	fread(&num, sizeof(unsigned int), 1, file);
	data[fileName].vertex.resize(num);
	for (auto itr = data[fileName].vertex.begin(); itr != data[fileName].vertex.end(); ++itr)
	{
		fread(&itr->pos,    sizeof(itr->pos),    1, file);
		fread(&itr->normal, sizeof(itr->normal), 1, file);
		fread(&itr->uv,     sizeof(itr->uv),     1, file);
		fread(&itr->bornNo, sizeof(itr->bornNo), 1, file);
		fread(&itr->weight, sizeof(itr->weight), 1, file);
		fread(&itr->edge,   sizeof(itr->edge),   1, file);
	}

	//���_�C���f�b�N�X�ǂݍ���
	fread(&num, sizeof(unsigned int), 1, file);
	data[fileName].index.resize(num);
	for (auto itr = data[fileName].index.begin(); itr != data[fileName].index.end(); ++itr)
	{
		fread(&(*itr), sizeof(unsigned short), 1, file);
	}

	//�}�e���A���̓ǂݍ���
	fread(&num, sizeof(unsigned int), 1, file);
	data[fileName].material.resize(num);
	fread(data[fileName].material.data(), sizeof(pmd::Material), num, file);

	//�{�[���̓ǂݍ���
	unsigned short bornNum = 0;
	fread(&bornNum, sizeof(unsigned short), 1, file);
	data[fileName].born.resize(bornNum);
	for (auto& i : data[fileName].born)
	{
		fread(&i.name,     sizeof(i.name),     1, file);
		fread(&i.pIndex,   sizeof(i.pIndex),   1, file);
		fread(&i.cIndex,   sizeof(i.cIndex),   1, file);
		fread(&i.type,     sizeof(i.type),     1, file);
		fread(&i.IKpIndex, sizeof(i.IKpIndex), 1, file);
		fread(&i.pos,      sizeof(i.pos),      1, file);
	}

	//IK 
	unsigned short ikNum = 0; fread(&ikNum, sizeof(ikNum), 1, file); //���`����������΂� 
	for (int i = 0; i < ikNum; ++i) {
		fseek(file, 4, SEEK_CUR);  unsigned char ikchainNum;//�Ԃ��E���� 
		fread(&ikchainNum, sizeof(ikchainNum), 1, file);
		fseek(file, 6, SEEK_CUR);  fseek(file, ikchainNum * sizeof(unsigned short), SEEK_CUR);
	}

	//�\�� 
	unsigned short skinNum = 0; fread(&skinNum, sizeof(skinNum), 1, file); //���R�̌����̂悤�ɂ�����΂��� 
	for (int i = 0; i < skinNum; ++i) {
		fseek(file, 20, SEEK_CUR);
		unsigned int vertNum = 0;
		fread(&vertNum, sizeof(vertNum), 1, file);
		fseek(file, 1, SEEK_CUR);  fseek(file, 16 * vertNum, SEEK_CUR);
	}

	//�\���p�\�� 
	unsigned char skinDispNum = 0;
	fread(&skinDispNum, sizeof(skinDispNum), 1, file); 
	fseek(file, skinDispNum * sizeof(unsigned short), SEEK_CUR);

	//�\���p�{�[���� 
	unsigned char boneDispNum = 0;
	fread(&boneDispNum, sizeof(boneDispNum), 1, file); 
	fseek(file, 50 * boneDispNum, SEEK_CUR);

	//�\���{�[�����X�g 
	unsigned int dispBoneNum = 0;
	fread(&dispBoneNum, sizeof(dispBoneNum), 1, file);
	fseek(file, 3 * dispBoneNum, SEEK_CUR);

	//�p�� //�p���Ή��t���O 
	unsigned char englishFlg = 0;
	fread(&englishFlg, sizeof(englishFlg), 1, file);
	if (englishFlg) {
		//���f����20�o�C�g+256�o�C�g�R�����g 
		fseek(file, 20 + 256, SEEK_CUR);
		//�{�[����20�o�C�g*�{�[���� 
		fseek(file, bornNum * 20, SEEK_CUR);
		//(�\�-1)*20�o�C�g�B-1�Ȃ̂̓x�[�X�����Ԃ� 
		fseek(file, (skinNum - 1) * 20, SEEK_CUR);
		//�{�[����*50�o�C�g�B 
		fseek(file, boneDispNum * 50, SEEK_CUR);
	}

	fread(toonName.data(), sizeof(char) * 100, toonName.size(), file);

	fclose(file);

	//�e�N�X�`���ǂݍ���
	LoadTex(dev, fileName);
	LoadToon(dev, fileName);

	//���_�o�b�t�@����
	CreateRsc(dev, &data[fileName].vRsc, sizeof(pmd::Vertex) * data[fileName].vertex.size());
	Map(&data[fileName].vRsc, data[fileName].vertex.data(), data[fileName].vertex.size(), &data[fileName].vertexData);

	//�C���f�b�N�X�o�b�t�@�̐���
	CreateRsc(dev, &data[fileName].iRsc, sizeof(unsigned short) * data[fileName].index.size());
	Map(&data[fileName].iRsc, data[fileName].index.data(), data[fileName].index.size(), &data[fileName].indexData);

	SetBornNode(fileName);

	return 0;
}
