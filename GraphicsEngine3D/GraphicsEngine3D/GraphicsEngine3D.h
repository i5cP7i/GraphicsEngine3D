#pragma once
#pragma comment(lib, "winmm.lib")

#ifndef UNICODE
#error
#endif

#include <windows.h>

#include <iostream>
#include <chrono>
#include <vector>
#include <thread>
#include <atomic>
#include <condition_variable>

class GraphicsEngine3D
{
public:
	GraphicsEngine3D();
	~GraphicsEngine3D();
public:
	enum class COLOUR //Foreground text color values
	{
		FG_BLACK = 0x0000,
		FG_DARK_BLUE = 0x0001,
		FG_DARK_GREEN = 0x0002,
		FG_DARK_CYAN = 0x0003,
		FG_DARK_RED = 0x0004,
		FG_DARK_MAGENTA = 0x0005,
		FG_DARK_YELLOW = 0x0006,
		FG_GREY = 0x0007, 
		FG_DARK_GREY = 0x0008,
		FG_BLUE = 0x0009,
		FG_GREEN = 0x000A,
		FG_CYAN = 0x000B,
		FG_RED = 0x000C,
		FG_MAGENTA = 0x000D,
		FG_YELLOW = 0x000E,
		FG_WHITE = 0x000F,
		BG_BLACK = 0x0000,
		BG_DARK_BLUE = 0x0010,
		BG_DARK_GREEN = 0x0020,
		BG_DARK_CYAN = 0x0030,
		BG_DARK_RED = 0x0040,
		BG_DARK_MAGENTA = 0x0050,
		BG_DARK_YELLOW = 0x0060,
		BG_GREY = 0x0070,
		BG_DARK_GREY = 0x0080,
		BG_BLUE = 0x0090,
		BG_GREEN = 0x00A0,
		BG_CYAN = 0x00B0,
		BG_RED = 0x00C0,
		BG_MAGENTA = 0x00D0,
		BG_YELLOW = 0x00E0,
		BG_WHITE = 0x00F0
	};
	enum class PIXEL_TYPE
	{
		PIXEL_SOLID = 0x2588,
		PIXEL_THREEQUARTERS = 0x2593,
		PIXEL_HALF = 0x2592,
		PIXEL_QUARTER = 0x2591
	};
public:
	int getConsoleWindowWidth() const;
	int getConsoleWindowHeight() const;
protected:
	int CreateConsoleWindow(int width, int height, int fontSizeX, int fontSizeY);
private:
	int ErrMsg(const wchar_t *msg);

	int c_ScreenWidth;
	int c_ScreenHeight;
	CHAR_INFO *c_bufScreen;
	std::wstring c_AppName;
	HANDLE c_OriginalConsole;
	HANDLE c_ConsoleOut;
	HANDLE c_ConsoleIn;
	CONSOLE_SCREEN_BUFFER_INFO c_OriginalConsoleInfo;

	SMALL_RECT c_rectWindow;
	COORD c_ConsoleSpace;
};

