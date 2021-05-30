#include "demo_button.h"


region_button::region_button()
{
    internInit();
}

region_button::~region_button()
{
}

void region_button::setBtnName()
{
    char* pName = getBtnName();
    strcat(pName, "REGION");
}

void region_button::setWndClass()
{
    char* pName = getWndClass();
    strcat(pName, "RegionControl");
}

void region_button::setWndMenu()
{
    HMENU* hmenu = getWndMenu();
    *hmenu = (HMENU)ID_REGION;
}

LRESULT CALLBACK regionControlProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return 0;
}

WNDPROC region_button::getWndProc()
{
    return regionControlProc;
}
