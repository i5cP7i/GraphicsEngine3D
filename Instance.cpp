#include "Instance.h"
#include "Initialize.h"
#include "Cube.h"

std::atomic<bool> Instance::bAtomicActive;
std::condition_variable Instance::EngineFinished;
std::mutex Instance::muxEngine;

Instance::Instance(unsigned char id) : GraphicsEngine3D (id)
{
    c_ScreenWidth = 320;
    c_ScreenHeight = 180;

    c_ConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    c_ConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

    c_AppName = L"Graphics Engine 3D";
}
Instance::~Instance()
{
    //delete cube;
    delete c_ScreenBuffer;
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
    c_rectWindow = { 0, 0, static_cast<SHORT>(c_ScreenWidth - 1), static_cast<SHORT>(c_ScreenHeight - 1)};
    SetConsoleWindowInfo(c_ConsoleOut, TRUE, &c_rectWindow);

    //Allocate memory for screen buffer
    c_ScreenBuffer = new CHAR_INFO[c_ScreenWidth * c_ScreenHeight];
    memset(c_ScreenBuffer, 0, sizeof(CHAR_INFO) * c_ScreenWidth * c_ScreenHeight);
    //Mouse Input
    GetConsoleCursorInfo(c_ConsoleOut, &c_ConsoleCursorInfo);
    c_ConsoleCursorInfo.bVisible = false;
    SetConsoleCursorInfo(c_ConsoleOut, &c_ConsoleCursorInfo);
    SetConsoleMode(c_ConsoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

    SetConsoleCtrlHandler(static_cast<PHANDLER_ROUTINE>(CloseHandler), TRUE);
    return 1;
}

// Create objects
bool Instance::EngineCreate()
{
    cube = new Cube(1.0f, 0.0f, 0.0f, 0.0f);
    cube->setProjectionMatrix(this, 0.1f, 1000.0f, 90.0f);
    return true;
}

// Update the screen
bool Instance::EngineUpdate(float fElapsedTime)
{
    Fill(0, 0, getConsoleWindowWidth(), getConsoleWindowHeight(), PIXEL_SOLID, FG_BLACK);
    // Engine code goes here
    cube->demoCube(this);
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

void Instance::StartThread(Instance &i)
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

void Instance::drawLine(int x1, int y1, int x2, int y2)
{
    drawLine(x1, y1, x2, y2, GraphicsEngine3D::PIXEL_TYPE::PIXEL_SOLID, GraphicsEngine3D::COLOR::FG_WHITE);
}

void Instance::drawLine(int x1, int y1, int x2, int y2, GraphicsEngine3D::PIXEL_TYPE pxt, GraphicsEngine3D::COLOR cl)
{
    int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
    dx = x2 - x1; dy = y2 - y1;
    dx1 = abs(dx); dy1 = abs(dy);
    px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
    if (dy1 <= dx1)
    {
        if (dx >= 0)
            { x = x1; y = y1; xe = x2; }
        else
            { x = x2; y = y2; xe = x1;}

        draw(x, y, pxt, cl);

        for (i = 0; x<xe; i++)
        {
            x = x + 1;
            if (px<0)
                px = px + 2 * dy1;
            else
            {
                if ((dx<0 && dy<0) || (dx>0 && dy>0)) y = y + 1; else y = y - 1;
                px = px + 2 * (dy1 - dx1);
            }
            draw(x, y, pxt, cl);
        }
    }
    else
    {
        if (dy >= 0)
            { x = x1; y = y1; ye = y2; }
        else
            { x = x2; y = y2; ye = y1; }

        draw(x, y, pxt, cl);

        for (i = 0; y<ye; i++)
        {
            y = y + 1;
            if (py <= 0)
                py = py + 2 * dx1;
            else
            {
                if ((dx<0 && dy<0) || (dx>0 && dy>0)) x = x + 1; else x = x - 1;
                py = py + 2 * (dx1 - dy1);
            }
            draw(x, y, pxt, cl);
        }
    }
}

void Instance::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    drawTriangle(x1, y1, x2, y2, x3, y3, GraphicsEngine3D::PIXEL_TYPE::PIXEL_SOLID, GraphicsEngine3D::COLOR::FG_WHITE);
}

void Instance::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, GraphicsEngine3D::PIXEL_TYPE pxt, GraphicsEngine3D::COLOR cl)
{
    drawLine(x1, y1, x2, y2, pxt, cl);
    drawLine(x2, y2, x3, y3, pxt, cl);
    drawLine(x3, y3, x1, y1, pxt, cl);
}

void Instance::drawCircle()
{
}

void Instance::draw(int x, int y)
{
    draw(x, y, GraphicsEngine3D::PIXEL_TYPE::PIXEL_SOLID, GraphicsEngine3D::COLOR::FG_WHITE);
}

void Instance::draw(int x, int y, GraphicsEngine3D::PIXEL_TYPE pxt, GraphicsEngine3D::COLOR cl)
{
    if ((x >= 0 && x < c_ScreenWidth) && (y >= 0 && y < c_ScreenHeight))
    {
        c_ScreenBuffer[y * getConsoleWindowWidth() + x].Char.UnicodeChar = pxt;
        c_ScreenBuffer[y * getConsoleWindowWidth() + x].Attributes = cl;
    }
}

void Instance::Fill(int x1, int y1, int x2, int y2)
{
    Fill(x1, y1, x2, y2, GraphicsEngine3D::PIXEL_TYPE::PIXEL_SOLID, GraphicsEngine3D::COLOR::FG_WHITE);
}

void Instance::Fill(int x1, int y1, int x2, int y2, GraphicsEngine3D::PIXEL_TYPE pxt, GraphicsEngine3D::COLOR cl)
{
    Clip(x1, y1);
    Clip(x2, y2);
    for (int x = x1; x < x2; x++)
        for (int y = y1; y < y2; y++)
            draw(x, y, pxt, cl);
}

void Instance::Clip(int &x, int &y)
{
    if (x < 0)
    {
        x = 0;
    }
    if (x >= getConsoleWindowWidth())
    {
        x = getConsoleWindowWidth();
    }
    if (y < 0)
    {
        y = 0;
    }
    if (y >= getConsoleWindowHeight())
    {
        y = getConsoleWindowHeight();
    }
}
