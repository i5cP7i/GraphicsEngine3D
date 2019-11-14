#pragma once
#include "GraphicsEngine3D.h"
#include "Polygon3D.h"
#include "Cube.h"

class Instance :
	public GraphicsEngine3D
{
	friend class Initialize;
private:
	void draw(int x, int y);
	void draw(int x, int y, GraphicsEngine3D::PIXEL_TYPE, GraphicsEngine3D::COLOR);
	void Fill(int x1, int y1, int x2, int y2);
	void Fill(int x1, int y1, int x2, int y2, GraphicsEngine3D::PIXEL_TYPE, GraphicsEngine3D::COLOR);
	void Clip(int &x, int &y);
public:
	Instance(unsigned char id = 0);
	virtual ~Instance();
<<<<<<< Updated upstream
	int getConsoleWindowWidth() const;
	int getConsoleWindowHeight() const;
protected:
	virtual int CreateConsoleWindow(int width, int height, int fontSizeX, int fontSizeY);
	[[nodiscard]]virtual inline volatile const std::wstring getConsoleAppName() const noexcept;
=======
	virtual int CreateConsoleWindow(int width, int height, int fontSizeX, int fontSizeY);
	bool EngineCreate() override;
	bool EngineUpdate(float fElapsedTime) override;
	int getConsoleWindowWidth() const;
	int getConsoleWindowHeight() const;
	void StartThread(Instance &i);

	//std::vector<Polygon3D *> getPolygons() const { return polygons; }

	float fElapsedTime;
protected:
	[[nodiscard]]virtual const std::wstring getConsoleAppName() const noexcept;
	static BOOL CloseHandler(DWORD evt);

	void drawLine(int x1, int x2, int y1, int y2);
	void drawLine(int x1, int x2, int y1, int y2, GraphicsEngine3D::PIXEL_TYPE, GraphicsEngine3D::COLOR);
	void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
	void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, GraphicsEngine3D::PIXEL_TYPE, GraphicsEngine3D::COLOR);
	void drawCircle();

>>>>>>> Stashed changes
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
	CONSOLE_CURSOR_INFO c_ConsoleCursorInfo;
	SMALL_RECT c_rectWindow;
	COORD c_ConsoleSpace;
<<<<<<< Updated upstream
	std::wstring c_AppName = L"GraphicsEngine3D";
=======
	std::wstring c_AppName = L"";

	void InstanceThread();
	static std::atomic<bool> bAtomicActive;
	static std::condition_variable EngineFinished;
	static std::mutex muxEngine;
>>>>>>> Stashed changes
};

