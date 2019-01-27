#include "Primitive.h"
#include "../Window/Window.h"
#include "../List/List.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../Release.h"

// �R���X�g���N�^
Primitive::Primitive(std::weak_ptr<Window> win, std::weak_ptr<Root> root, std::weak_ptr<Pipe> pipe, const size_t & num) : win(win), root(root), pipe(pipe),
	rsc(nullptr)
{
}

// �f�X�g���N�^
Primitive::~Primitive()
{
	Release(rsc);
}

// ���_���\�[�X�̐���
long Primitive::CreateVRsc(const size_t & num)
{
	return 0;
}
