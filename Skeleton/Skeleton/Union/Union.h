#pragma once
#include "../etc/tString.h"
#include <map>
#include <list>
#include <vector>
#include <memory>

enum D3D12_PRIMITIVE_TOPOLOGY_TYPE : int;
class RootMane;
class PipeMane;
class Window;
class Device;
class Queue;
class List;
class Swap;
class Render;
class Depth;
class Fence;
class MultiPass;
class ShadowMap;
class Texture;
class Point;
class Line;
class Triangle;
class Camera;
class Plane;
class Pmd;

class Union
{
public:
	// �R���X�g���N�^
	Union(std::weak_ptr<Window>win);
	// �f�X�g���N�^
	~Union();

	// �J�����̈ʒu�E�^�[�Q�b�g
	void SetCamera(const float& x, const float& y, const float& z, const float& targetX, const float& targetY, const float& targetZ);

	// �摜�̓ǂݍ���
	void LoadImg(const std::string& fileName, int& i);

	// �z��f�[�^����摜�𐶐�
	void CreateImg(const std::vector<unsigned char>&data, const unsigned int& width, const unsigned int& height, int& i);

	// �摜�̕`��
	void DrawImg(int& i, const float& x, const float& y, const float& sizeX, const float& sizeY, const float& rectX, const float& rectY, 
		const float& rectSizexX, const float& rectSizeY, const float& alpha = 1.0f, const bool& turnX = false, const bool& turnY = false);

	// �_�̕`��
	void DrawPoint(const float& x, const float& y, const float& r, const float& g, const float& b, const float& alpha = 1.0f);

	// ���̕`��
	void DrawLine(const float& x1, const float& y1, const float& x2, const float& y2, 
		const float& r, const float& g, const float& b, const float& alpha = 1.0f);

	// �O�p�`�̕`��
	void DrawTriangle(const float& x1, const float& y1, const float& x2, const float& y2, const float& x3, const float& y3,
		const float& r, const float& g, const float& b, const float& alpha = 1.0f);

	// �l�p�`�̕`��
	void DrawBox(const float& x, const float& y, const float& sizeX, const float& sizeY,
		const float& r, const float& g, const float& b, const float& alpha = 1.0f);

	// ���̕`��
	void DrawPlane(const float& x, const float& y, const float& z, const float& sizeX, const float& sizeY, const float& sizeZ,
		const float& r, const float& g, const float& b, const float& alpha = 1.0f);

	// PMD�̓ǂݍ���
	void LoadPmd(const std::string& fileName, int& i);

	// PMD��Y����]
	void RotatePmd(int& i, const float& angle);

	// PMD�̕`��
	void DrawPmd(int& i);

	// PMD�̉e�`��
	void DrawPmdShadow(int& i);
	
	// ���[�V�����̓K��
	void Attach(const std::string& fileName, int& i);

	// �A�j���[�V����
	void Animation(int& i, const bool& loop = true, const float& animSpeed = 1.0f);

	// �e���̃N���A
	void ClearShadow(void);

	// �e�̎��s
	void ExecutionShadow(void);

	// ��ʃN���A
	void Clear(void);

	// ���s
	void Execution(void);

	// �f�o�C�X�̎擾
	std::shared_ptr<Device>GetDev(void) const {
		return dev;
	}

private:
	// ���[�g�V�O�l�`���̐���
	void CreateRoot(const std::string& name, const std::tstring& fileName);
	void CreateRoot(void);

	// �p�C�v���C���̐���
	void CreatePipe(const std::string& name, const std::string& rootName, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& type,
		const std::initializer_list<int>&index, const bool& depth = false);
	void CreatePipe(void);

	// �N���X�̐���
	void Create(void);

	// ���[�g�}�l�W���[
	RootMane& root;

	// �p�C�v�}�l�W���[
	PipeMane& pipe;

	// �E�B���h�E
	std::weak_ptr<Window>win;

	// �f�o�C�X
	std::shared_ptr<Device>dev;

	// �L���[
	std::shared_ptr<Queue>queue;

	// ���X�g
	std::shared_ptr<List>list;

	// �X���b�v
	std::shared_ptr<Swap>swap;

	// �����_�[
	std::unique_ptr<Render>ren;

	// �[�x
	std::shared_ptr<Depth>depth;

	// �t�F���X
	std::shared_ptr<Fence>fence;

	// �}���`�p�X
	std::unique_ptr<MultiPass>multi;

	// �V���h�E
	std::shared_ptr<ShadowMap>shadow;

	// ���[�g�V�O�l�`��ID
	std::map<std::string, int>rootNo;

	// �p�C�v���C��ID
	std::map<std::string, int>pipeNo;

	// �e�N�X�`��
	std::unique_ptr<Texture>tex;

	// �|�C���g
	std::list<std::shared_ptr<Point>>point;

	// ���C��
	std::list<std::shared_ptr<Line>>line;

	// �g���C�A���O��
	std::list<std::shared_ptr<Triangle>>triangle;

	// �J����
	std::shared_ptr<Camera>cam;

	// ��
	std::list<std::shared_ptr<Plane>>plane;

	// PMD
	std::unique_ptr<Pmd>pmd;
};
