#include "demo_button.h"


preview_button::preview_button()
{
    internInit();
}

preview_button::~preview_button()
{
}

void preview_button::setBtnName()
{
    char* pName = getBtnName();
    strcat(pName, "PREVIEW");
}

void preview_button::setWndClass()
{
    char* pName = getWndClass();
    strcat(pName, "PreviewControl");
}

void preview_button::setWndMenu()
{
    HMENU* hmenu = getWndMenu();
    *hmenu = (HMENU)ID_PREVIEW;
}

LRESULT CALLBACK previewControlProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return 0;
}

WNDPROC preview_button::getWndProc()
{
    return previewControlProc;
}
