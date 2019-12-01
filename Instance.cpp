#include "Instance.h"
#include "Initialize.h"
#include "Cube.h"
#include "Object3D.h"

//statics
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
    //cube = new Cube(1.0f, 0.0f, 0.0f, 0.0f);
    //cube->setProjectionMatrix(this, 0.1f, 1000.0f, 90.0f);
    object3d = new Object3D("C:/Users/11703088/OneDrive - PXL/Electronic_Engineering/EAI2/C++/Individual_project/GraphicsEngine3D/GraphicsEngine3D/pyramid.obj");
    object3d->setProjectionMatrix(this, 0.1f, 1000.0f, 90.0f);
    return true;
}

// Update the screen
bool Instance::EngineUpdate(float fElapsedTime)
{
    Fill(0, 0, getConsoleWindowWidth(), getConsoleWindowHeight(), PIXEL_SOLID, FG_BLUE);
    // Engine code goes here
    object3d->demoCustomObject(this);
    //cube->demoCube(this);
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
void Instance::FillTriangle(Instance *instance, int x1, int y1, int x2, int y2, int x3, int y3)
{
    FillTriangle(instance, x1, y1, x2, y2, x3, y3, GraphicsEngine3D::PIXEL_TYPE::PIXEL_SOLID, GraphicsEngine3D::COLOR::FG_WHITE);
}
void Instance::FillTriangle(Instance *instance, int x1, int y1, int x2, int y2, int x3, int y3, GraphicsEngine3D::PIXEL_TYPE pxt, GraphicsEngine3D::COLOR cl)
{
    auto SWAP = [](int &x, int &y) { int t = x; x = y; y = t; };
    auto drawline = [&](int sx, int ex, int ny)
    {
        for (int i = sx; i <= ex; i++)
        {
            instance->draw(i, ny, pxt, cl);
        }
        return 1;
    };

    int t1x, t2x, y, minx, maxx, t1xp, t2xp;
    bool changed1 = false;
    bool changed2 = false;
    int signx1, signx2, dx1, dy1, dx2, dy2;
    int e1, e2;
    // Sort vertices
    if (y1>y2) { SWAP(y1, y2); SWAP(x1, x2); }
    if (y1>y3) { SWAP(y1, y3); SWAP(x1, x3); }
    if (y2>y3) { SWAP(y2, y3); SWAP(x2, x3); }

    t1x = t2x = x1; y = y1;   // Starting points
    dx1 = (int)(x2 - x1); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
    else signx1 = 1;
    dy1 = (int)(y2 - y1);

    dx2 = (int)(x3 - x1); if (dx2<0) { dx2 = -dx2; signx2 = -1; }
    else signx2 = 1;
    dy2 = (int)(y3 - y1);

    if (dy1 > dx1) {   // swap values
        SWAP(dx1, dy1);
        changed1 = true;
    }
    if (dy2 > dx2) {   // swap values
        SWAP(dy2, dx2);
        changed2 = true;
    }

    e2 = (int)(dx2 >> 1);
    // Flat top, just process the second half
    if (y1 == y2) goto next;
    e1 = (int)(dx1 >> 1);

    for (int i = 0; i < dx1;) {
        t1xp = 0; t2xp = 0;
        if (t1x<t2x) { minx = t1x; maxx = t2x; }
        else { minx = t2x; maxx = t1x; }
        // process first line until y value is about to change
        while (i<dx1) {
            i++;
            e1 += dy1;
            while (e1 >= dx1) {
                e1 -= dx1;
                if (changed1) t1xp = signx1;//t1x += signx1;
                else          goto next1;
            }
            if (changed1) break;
            else t1x += signx1;
        }
        // Move line
    next1:
        // process second line until y value is about to change
        while (1) {
            e2 += dy2;
            while (e2 >= dx2) {
                e2 -= dx2;
                if (changed2) t2xp = signx2;//t2x += signx2;
                else          goto next2;
            }
            if (changed2)     break;
            else              t2x += signx2;
        }
    next2:
        if (minx>t1x) minx = t1x; if (minx>t2x) minx = t2x;
        if (maxx<t1x) maxx = t1x; if (maxx<t2x) maxx = t2x;
        drawline(minx, maxx, y);    // Draw line from min to max points found on the y
                                     // Now increase y
        if (!changed1) t1x += signx1;
        t1x += t1xp;
        if (!changed2) t2x += signx2;
        t2x += t2xp;
        y += 1;
        if (y == y2) break;

    }
next:
    // Second half
    dx1 = (int)(x3 - x2); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
    else signx1 = 1;
    dy1 = (int)(y3 - y2);
    t1x = x2;

    if (dy1 > dx1) {   // swap values
        SWAP(dy1, dx1);
        changed1 = true;
    }
    else changed1 = false;

    e1 = (int)(dx1 >> 1);

    for (int i = 0; i <= dx1; i++) {
        t1xp = 0; t2xp = 0;
        if (t1x<t2x) { minx = t1x; maxx = t2x; }
        else { minx = t2x; maxx = t1x; }
        // process first line until y value is about to change
        while (i<dx1) {
            e1 += dy1;
            while (e1 >= dx1) {
                e1 -= dx1;
                if (changed1) { t1xp = signx1; break; }//t1x += signx1;
                else          goto next3;
            }
            if (changed1) break;
            else   	   	  t1x += signx1;
            if (i<dx1) i++;
        }
    next3:
        // process second line until y value is about to change
        while (t2x != x3) {
            e2 += dy2;
            while (e2 >= dx2) {
                e2 -= dx2;
                if (changed2) t2xp = signx2;
                else          goto next4;
            }
            if (changed2)     break;
            else              t2x += signx2;
        }
    next4:

        if (minx>t1x) minx = t1x; if (minx>t2x) minx = t2x;
        if (maxx<t1x) maxx = t1x; if (maxx<t2x) maxx = t2x;
        drawline(minx, maxx, y);
        if (!changed1) t1x += signx1;
        t1x += t1xp;
        if (!changed2) t2x += signx2;
        t2x += t2xp;
        y += 1;
        if (y>y3) return;
    }
}

CHAR_INFO Instance::getColorFromLux(float lux)
{
        uint16_t bg_col, fg_col;
        float multiplier = 23.0f;
        wchar_t sym;
        int pixel_bw = (int)(multiplier*lux);
        switch (pixel_bw)
        {
        case 0: bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID; break;

        case 1: bg_col = BG_BLACK; fg_col = FG_DARK_GRAY; sym = PIXEL_QUARTER; break;
        case 2: bg_col = BG_BLACK; fg_col = FG_DARK_GRAY; sym = PIXEL_HALF; break;
        case 3: bg_col = BG_BLACK; fg_col = FG_DARK_GRAY; sym = PIXEL_THREEQUARTERS; break;
        case 4: bg_col = BG_BLACK; fg_col = FG_DARK_GRAY; sym = PIXEL_SOLID; break;

        case 5: bg_col = BG_DARK_GRAY; fg_col = FG_GRAY; sym = PIXEL_QUARTER; break;
        case 6: bg_col = BG_DARK_GRAY; fg_col = FG_GRAY; sym = PIXEL_HALF; break;
        case 7: bg_col = BG_DARK_GRAY; fg_col = FG_GRAY; sym = PIXEL_THREEQUARTERS; break;
        case 8: bg_col = BG_DARK_GRAY; fg_col = FG_GRAY; sym = PIXEL_SOLID; break;

        case 9:  bg_col = BG_GRAY; fg_col = FG_WHITE; sym = PIXEL_QUARTER; break;
        case 10: bg_col = BG_GRAY; fg_col = FG_WHITE; sym = PIXEL_HALF; break;
        case 11: bg_col = BG_GRAY; fg_col = FG_WHITE; sym = PIXEL_THREEQUARTERS; break;
        case 12: bg_col = BG_GRAY; fg_col = FG_WHITE; sym = PIXEL_SOLID; break;
        default:
            bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID;
        }

        CHAR_INFO c;
        c.Attributes = bg_col | fg_col;
        c.Char.UnicodeChar = sym;
        return c;
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
