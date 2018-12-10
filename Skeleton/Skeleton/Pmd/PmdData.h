#pragma once
#include <vector>
#include <DirectXMath.h>

namespace pmd {
#pragma pack(1)
	// �w�b�_�[
	struct Header {
		//�^�C�v
		unsigned char type[3];
		//�o�[�W����
		float ver;
		//���O
		unsigned char name[20];
		//�R�����g
		unsigned char comment[256];
	};
#pragma pack()

	// ���_�f�[�^
	struct Vertex {
		//���W
		DirectX::XMFLOAT3 pos;
		//�@��
		DirectX::XMFLOAT3 normal;
		//UV
		DirectX::XMFLOAT2 uv;
		//�{�[���ԍ�
		unsigned short bornNo[2];
		//�E�F�C�g
		unsigned char weight;
		//�֊s���t���O
		unsigned char edge;
	};

#pragma pack(1)
	// �}�e���A��
	struct Material {
		//��{�F
		DirectX::XMFLOAT3 diffuse;
		//�����x
		float alpha;
		//���ˋ��x
		float specularity;
		//���ːF
		DirectX::XMFLOAT3 specula;
		//���F
		DirectX::XMFLOAT3 mirror;
		//�g�D�[���ԍ�
		unsigned char toonIndex;
		//�֊s���t���O
		unsigned char edge;
		//�C���f�b�N�X��
		unsigned int indexNum;
		//�e�N�X�`���p�X
		unsigned char texPath[20];
	};
#pragma pack()

	// �{�[��
	struct Born {
		//���O
		char name[20];
		//�e�{�[���ԍ�
		unsigned short pIndex;
		//�q�{�[���ԍ�
		unsigned short cIndex;
		//�^�C�v
		unsigned char type;
		//IK�e�{�[���ԍ�
		unsigned short IKpIndex;
		//���W
		DirectX::XMFLOAT3 pos;
	};

	// ���M���̃}�e���A�����
	struct Mat {
		//��{�F
		DirectX::XMFLOAT3 diffuse;
		//�����x
		float alpha;
		//���ˋ��x
		float specularity;
		//���ːF
		DirectX::XMFLOAT3 specula;
		//���F
		DirectX::XMFLOAT3 mirror;
		//�e�N�X�`���t���O
		int tex;
		//���Z�e�N�X�`���t���O
		int spa;
		//��Z�e�N�X�`���t���O
		int sph;
	};

	// �{�[���m�[�h
	struct BornNode {
		//�z��ԍ�
		unsigned int index;
		//�{�[���n�_
		DirectX::XMFLOAT3 start;
		//�q�{�[��
		std::vector<BornNode*>child;
	};
}
