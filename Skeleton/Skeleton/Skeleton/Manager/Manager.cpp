#include "Manager.h"
#include "../Application/Application.h"
#include "../Mouse/Mouse.h"

// �R���X�g���N�^
Manager::Manager() : 
	pos(0.0f), size(0.0f)
{
	imag.clear();
}

// �f�X�g���N�^
Manager::~Manager()
{
}

// �ǂݍ���
void Manager::Load(const std::string & name, const std::string & fileName, const Vec2f & uvSize, const Vec2f & uvPos)
{
	if (imag.find(name) != imag.end())
	{
		return;
	}
	app.lock()->LoadTex(imag[name].id, fileName);
	imag[name].pos  = uvPos;
	imag[name].size = uvSize;
}

// �`��
void Manager::DrawImg(const std::string & name, const Vec2f & pos, const Vec2f & size, const float & alpha)
{
	if (imag.find(name) == imag.end())
	{
		return;
	}
	app.lock()->DrawTex(imag[name].id, pos, size, imag[name].pos, imag[name].size, alpha, imag[name].angle);
}

// �}�E�X���͈͂ɓ����Ă��邩�`�F�b�N
bool Manager::CheckMouse(const Vec2f & pos, const Vec2f & size)
{
	if ((pos.x < mouse.lock()->GetClick().x && mouse.lock()->GetClick().x < pos.x + size.x)
		&& (pos.y < mouse.lock()->GetClick().y && mouse.lock()->GetClick().y < pos.y + size.y))
	{
		return true;
	}

	return false;
}

// �p�x�̍X�V
void Manager::UpDataAngle(const std::string & name, const float & now, const float & max)
{
	if (imag.find(name) == imag.end())
	{
		return;
	}

	imag[name].angle = PIF - (PIF * -(now / max));
}

// �I��
void Manager::End(void)
{
	for (auto itr = imag.begin(); itr != imag.end(); ++itr)
	{
		app.lock()->DeleteTex(itr->second.id);
	}
}
