#include "Mouse.h"
#include "../Application/Application.h"

// コンストラクタ
Mouse::Mouse(std::weak_ptr<Application> app) : app(app), 
	click(-1), pos(-1)
{
}

// デストラクタ
Mouse::~Mouse()
{
}

// 処理
void Mouse::UpData(void)
{
	if (Input::Get().CheckKey(INPUT_LBUTTON))
	{
		if (click == -1)
		{
			click = app.lock()->GetMousePos();
		}
		pos = app.lock()->GetMousePos();
	}
	else
	{
		click = -1;
		pos   = click;
	}
}
