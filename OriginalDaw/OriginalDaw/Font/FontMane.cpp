#include "FontMane.h"
#include "FontLoader.h"
#include "../Root/Root.h"
#include "../Pipe/PipeMane.h"

// コンストラクタ
FontMane::FontMane(std::weak_ptr<Root> root) : root(root)
{
	//PipeMane::Get().Create("font", root, )
}

// デストラクタ
FontMane::FontMane()
{
	PipeMane::Get().Delete("font");
}
