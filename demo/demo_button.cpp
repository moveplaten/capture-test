#include "demo_button.h"


_button::_button()
{
    width = 80;
    height = 25;

    initial_style = WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | BS_TEXT | BS_PUSHBUTTON | WS_CLIPSIBLINGS;
}

_button::~_button()
{
}


demo_button::demo_button()
{
    hwndSelf = NULL;
    isClicked = false;
    hwndMenu = NULL;
    hinstMain = NULL;

    btnName = (char*)malloc(20);
    ZeroMemory(btnName, 20);
    wndClass = (char*)malloc(20);
    ZeroMemory(wndClass, 20);
}

demo_button::~demo_button()
{
}


void demo_button::internInit(char* _btnName, char* _wndClass, HMENU _hwndMenu)
{
    HMENU* __hwndMenu = getWndMenu();
    strcat(getBtnName(), _btnName);
    strcat(getWndClass(), _wndClass);
    *__hwndMenu = _hwndMenu;

    setHinstMain();

    if (!initControl()) exit(1);
}

bool demo_button::initControl()
{
    WNDCLASS wc;
    WORD res = 0;
    ZeroMemory(&wc, sizeof(wc));

    wc.cbClsExtra = 0;
    wc.cbWndExtra = sizeof(LPVOID);
    wc.hbrBackground = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = NULL;
    wc.hInstance = getInstance();
    wc.lpfnWndProc = getWndProc();
    wc.lpszClassName = getWndClass();
    wc.lpszMenuName = NULL;
    wc.style = CS_PARENTDC | CS_VREDRAW | CS_HREDRAW;

    res = RegisterClass(&wc);

    if (!res) return false;

    return true;
}

void demo_button::setHwndSelf(HWND hwnd)
{
    hwndSelf = hwnd;
}

void demo_button::setHinstMain()
{
    hinstMain = GetModuleHandle(0);
}

void demo_button::reverseClickStat()
{
    if (isClicked) isClicked = false;
    else isClicked = true;
}

bool demo_button::createWndSelf(HWND hwndMain)
{
    HWND hwndTemp;

    hwndTemp = CreateWindow(TEXT("BUTTON"), getBtnName(),
        getInitStyle(), 0, 0, 0, 0, hwndMain, *getWndMenu(), 0, 0);

    if (!hwndTemp) return false;

    SendMessage(hwndTemp, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);

    setHwndSelf(hwndTemp);
    return true;
}
