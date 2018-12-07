#pragma once
#include <string>
#include <memory>
#include <unordered_map>

struct D3D12_SUBRESOURCE_DATA;
struct ID3D12Resource;
class Device;

class TextureLoader
{
	// ���f�[�^
	struct Origin {
		//���\�[�X
		ID3D12Resource* rsc;
		//���_���\�[�X
		ID3D12Resource* vRsc;
		//�f�R�[�h
		std::unique_ptr<unsigned char[]>decode;
		//�T�u�f�[�^
		std::shared_ptr<D3D12_SUBRESOURCE_DATA>sub;
	};

public:
	// �f�X�g���N�^
	~TextureLoader();

	// �C���X�^���X�ϐ��̎擾
	static TextureLoader& Get(void) {
		static TextureLoader instance;
		return instance;
	}

	// �ǂݍ���
	long Load(std::weak_ptr<Device>dev, const std::string& fileName);

	// �摜�̉����̎擾
	unsigned int GetWidth(const std::string& fileName);

	// �摜�̉����̎擾
	unsigned int GetHeight(const std::string& fileName);

	// �f�[�^�̍폜
	void Delete(const std::string& fileName);

	// ���\�[�X�̎擾
	ID3D12Resource* GetRsc(const std::string& fileName) {
		return origin[fileName].rsc;
	}
	// �f�R�[�h�̎擾
	unsigned char* GetDecode(const std::string& fileName) {
		return origin[fileName].decode.get();
	}
	// �T�u�f�[�^�̎擾
	std::shared_ptr<D3D12_SUBRESOURCE_DATA>GetSub(const std::string& fileName) {
		return origin[fileName].sub;
	}

private:
	// �R���X�g���N�^
	TextureLoader();
	TextureLoader(const TextureLoader&)    = delete;
	void operator=(const TextureLoader&) = delete;


	// �I���W���f�[�^
	std::unordered_map<std::string, Origin>origin;
};
