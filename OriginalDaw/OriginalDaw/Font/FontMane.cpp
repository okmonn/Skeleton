#include "FontMane.h"
#include "FontLoader.h"
#include "../Root/Root.h"
#include "../Pipe/PipeMane.h"

// �R���X�g���N�^
FontMane::FontMane(std::weak_ptr<Root> root) : root(root)
{
	//PipeMane::Get().Create("font", root, )
}

// �f�X�g���N�^
FontMane::FontMane()
{
	PipeMane::Get().Delete("font");
}
