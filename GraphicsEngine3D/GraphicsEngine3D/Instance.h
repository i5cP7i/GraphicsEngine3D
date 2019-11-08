#pragma once
#include "GraphicsEngine3D.h"
class Instance :
	public GraphicsEngine3D
{
public:
	Instance(unsigned char instance = 0);
	virtual ~Instance();
	int getConsoleWindowWidth() const;
	int getConsoleWindowHeight() const;
protected:
	virtual int CreateConsoleWindow(int width, int height, int fontSizeX, int fontSizeY);
	[[nodiscard]]virtual inline volatile const std::wstring getConsoleAppName() const noexcept;
private:
	int ErrMsg(const wchar_t *msg);
	int c_ScreenWidth;
	int c_ScreenHeight;
	CHAR_INFO *c_ScreenBuffer;
	HANDLE c_OriginalConsole;
	HANDLE c_ConsoleOut;
	HANDLE c_ConsoleIn;
	CONSOLE_SCREEN_BUFFER_INFO c_OriginalConsoleInfo;
	CONSOLE_FONT_INFOEX c_ConsoleFontIndex;
	CONSOLE_SCREEN_BUFFER_INFO c_ConsoleScreenBufferInfo;
	SMALL_RECT c_rectWindow;
	COORD c_ConsoleSpace;
	std::wstring c_AppName = L"GraphicsEngine3D";
};

