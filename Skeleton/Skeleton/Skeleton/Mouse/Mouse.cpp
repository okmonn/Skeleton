#include "Mouse.h"
#include "../Application/Application.h"

// �R���X�g���N�^
Mouse::Mouse(std::weak_ptr<Application> app) : app(app), 
	click(-1), pos(-1)
{
}

// �f�X�g���N�^
Mouse::~Mouse()
{
}

// ����
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
