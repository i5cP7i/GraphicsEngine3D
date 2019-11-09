#pragma once
#include "GraphicsEngine3D.h"
class Instance :
	public GraphicsEngine3D
{
public:
	Instance(unsigned char instance = 0);
	virtual ~Instance();
	virtual int CreateConsoleWindow(int width, int height, int fontSizeX, int fontSizeY);
	virtual bool EngineCreate();
	virtual bool EngineUpdate(float fElapsedTime);
	int getConsoleWindowWidth() const;
	int getConsoleWindowHeight() const;
	void StartThread(Instance &i) const;
protected:
	[[nodiscard]]virtual const std::wstring getConsoleAppName() const noexcept;
	static BOOL CloseHandler(DWORD evt);
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

	void InstanceThread();
	static std::atomic<bool> bAtomicActive;
	static std::condition_variable EngineFinished;
	static std::mutex muxEngine;

	float fElapsedTime;
};

