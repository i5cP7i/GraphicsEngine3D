#include "GraphicsEngine3D.h"

GraphicsEngine3D::GraphicsEngine3D()
{
	c_ScreenWidth = 320;
	c_ScreenHeight = 180;

	c_ConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	c_ConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

	c_AppName = L"Graphics Engine 3D";
}


GraphicsEngine3D::~GraphicsEngine3D()
{

}

int GraphicsEngine3D::CreateConsoleWindow(int width, int height, int fontSizeX, int fontSizeY)
{
	//TODO error
	if (c_ConsoleOut == INVALID_HANDLE_VALUE)
	{
		
	}
	c_rectWindow = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(c_ConsoleOut, TRUE, &c_rectWindow);

	return 0;
}

int GraphicsEngine3D::getConsoleWindowWidth() const
{
	return c_ScreenWidth;
}

int GraphicsEngine3D::getConsoleWindowHeight() const
{
	return c_ScreenHeight;
}

int GraphicsEngine3D::ErrMsg(const wchar_t * msg)
{
	//TODO
	return 0;
}
