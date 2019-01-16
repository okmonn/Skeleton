#include "FontMane.h"
#include "FontLoader.h"
#include "../Font/Font.h"
#include "../Window/Window.h"
#include "../Root/Root.h"
#include "../Pipe/PipeMane.h"

// コンストラクタ
FontMane::FontMane(std::weak_ptr<Window> win, std::weak_ptr<Root> root) : win(win), root(root),
	handle(FontLoader::Get().GetFont("ＭＳ Ｐ明朝", 32)), size(32)
{
	//PipeMane::Get().Create("font", root, )
	font.push_back(std::make_shared<Font>(win, handle, "a"));
}

// デストラクタ
FontMane::FontMane()
{
	PipeMane::Get().Delete("font");
}
