#include "demo_button.h"


setting_button::setting_button()
{
    internInit();
}

setting_button::~setting_button()
{
}

void setting_button::setBtnName()
{
    char* pName = getBtnName();
    strcat(pName, "SETTING");
}

void setting_button::setWndClass()
{
    char* pName = getWndClass();
    strcat(pName, "SettingControl");
}

void setting_button::setWndMenu()
{
    HMENU* hmenu = getWndMenu();
    *hmenu = (HMENU)ID_SETTING;
}

LRESULT CALLBACK settingControlProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return 0;
}

WNDPROC setting_button::getWndProc()
{
    return settingControlProc;
}
