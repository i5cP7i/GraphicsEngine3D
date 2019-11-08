#include "Instance.h"
#include "GraphicsEngine3D.h"

Instance::Instance(unsigned char instance) : GraphicsEngine3D (instance)
{
	c_ScreenWidth = 320;
	c_ScreenHeight = 180;

	c_ConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	c_ConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

	c_AppName = L"Graphics Engine 3D";
}


Instance::~Instance()
{

}

int Instance::CreateConsoleWindow(int width, int height, int fontSizeX, int fontSizeY)
{
	//TODO error
	if (c_ConsoleOut == INVALID_HANDLE_VALUE)
	{

	}
	//Set console visual size
	c_rectWindow = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(c_ConsoleOut, TRUE, &c_rectWindow);
	//Set the size of the screenbuffer
	c_ConsoleSpace = { static_cast<short>(c_ScreenWidth), static_cast<short>(c_ScreenHeight) };
	SetConsoleScreenBufferSize(c_ConsoleOut, c_ConsoleSpace);

	//Set the font size
	c_ConsoleFontIndex.cbSize = sizeof(c_ConsoleFontIndex);
	c_ConsoleFontIndex.nFont = 0;
	c_ConsoleFontIndex.dwFontSize.X = fontSizeX;
	c_ConsoleFontIndex.dwFontSize.Y = fontSizeY;
	c_ConsoleFontIndex.FontFamily = FF_DONTCARE;
	c_ConsoleFontIndex.FontWeight = FW_NORMAL;

	//Set console font to Raster Font
	wcscpy_s(c_ConsoleFontIndex.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(c_ConsoleOut, false, &c_ConsoleFontIndex);

	//Get screen buffer info and check the maximum allowed window size.
	GetConsoleScreenBufferInfo(c_ConsoleOut, &c_ConsoleScreenBufferInfo);
	//TODO: ERROR CODE

	//Set physical console window size
	c_rectWindow = { 0, 0, static_cast<short>(c_ScreenWidth) - 1, static_cast<short>(c_ScreenHeight) - 1 };

	//Allocate memory for screen buffer
	c_ScreenBuffer = new CHAR_INFO[c_ScreenWidth * c_ScreenHeight];
	memset(c_ScreenBuffer, 0, sizeof(CHAR_INFO) * c_ScreenWidth * c_ScreenHeight);

	return 1;
}

inline volatile const std::wstring Instance::getConsoleAppName() const noexcept
{
	return c_AppName;
}

int Instance::getConsoleWindowWidth() const
{
	return c_ScreenWidth;
}

int Instance::getConsoleWindowHeight() const
{
	return c_ScreenHeight;
}

int Instance::ErrMsg(const wchar_t * msg)
{
	//TODO
	return 0;
}
