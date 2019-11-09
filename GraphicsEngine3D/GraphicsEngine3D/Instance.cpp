#include "Instance.h"
#include "GraphicsEngine3D.h"

std::atomic<bool> Instance::bAtomicActive;
std::condition_variable Instance::EngineFinished;
std::mutex Instance::muxEngine;

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
	delete[] c_ScreenBuffer;
	SetConsoleActiveScreenBuffer(c_OriginalConsole);
	EngineFinished.notify_one();
}

int Instance::CreateConsoleWindow(int width, int height, int fontSizeX, int fontSizeY)
{
	//TODO error
	if (c_ConsoleOut == INVALID_HANDLE_VALUE)
	{

	}
	c_ScreenWidth = width;
	c_ScreenHeight = height;

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
	c_rectWindow = { 0, 0, static_cast<short>(c_ScreenWidth) - 1, static_cast<short>(c_ScreenHeight) - 1};
	SetConsoleWindowInfo(c_ConsoleOut, TRUE, &c_rectWindow);

	//Allocate memory for screen buffer
	c_ScreenBuffer = new CHAR_INFO[c_ScreenWidth * c_ScreenHeight];
	memset(c_ScreenBuffer, 0, sizeof(CHAR_INFO) * c_ScreenWidth * c_ScreenHeight);
	//Mouse Input
	SetConsoleMode(c_ConsoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CloseHandler, TRUE);
	return 1;
}

bool Instance::EngineCreate()
{
	return true;
}

bool Instance::EngineUpdate(float fElapsedTime)
{
	return true;
}

const std::wstring Instance::getConsoleAppName() const noexcept
{
	return c_AppName;
}

BOOL Instance::CloseHandler(DWORD evt)
{
	if (evt == CTRL_CLOSE_EVENT)
	{
		bAtomicActive = false;
		std::unique_lock<std::mutex> ul(muxEngine);
		EngineFinished.wait(ul);
	}
	return true;
}

int Instance::getConsoleWindowWidth() const
{
	return c_ScreenWidth;
}

int Instance::getConsoleWindowHeight() const
{
	return c_ScreenHeight;
}

void Instance::StartThread(Instance &i) const 
{
	bAtomicActive = true;
	std::thread t = std::thread(&Instance::InstanceThread, i);
	t.join();
}

void Instance::InstanceThread()
{
	if (!EngineCreate())
		bAtomicActive = false;

	auto t1 = std::chrono::system_clock::now();
	auto t2 = std::chrono::system_clock::now();

	while (bAtomicActive)
	{
		t2 = std::chrono::system_clock::now();
		std::chrono::duration<float> delta_t = t2 - t1;
		t1 = t2;

		fElapsedTime = delta_t.count();

		if (!EngineUpdate(fElapsedTime))
		{
			bAtomicActive = false;
		}

		wchar_t str[256];
		swprintf_s(str, 256, L"Graphics Engine 3D - %s - FPS: %3.2f", getConsoleAppName().c_str(), 1.0f/fElapsedTime);
		SetConsoleTitle(str);
		WriteConsoleOutput(c_ConsoleOut, c_ScreenBuffer, { static_cast<short>(c_ScreenWidth), static_cast<short>(c_ScreenHeight) }, { 0, 0 }, &c_rectWindow);
	}
}

int Instance::ErrMsg(const wchar_t * msg)
{
	//TODO
	return 0;
}
