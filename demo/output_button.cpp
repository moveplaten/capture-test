#include "demo_button.h"


output_button::output_button()
{
    internInit();
}

output_button::~output_button()
{
}

void output_button::setBtnName()
{
    char* pName = getBtnName();
    strcat(pName, "OUTPUT");
}

void output_button::setWndClass()
{
    char* pName = getWndClass();
    strcat(pName, "OutputControl");
}

void output_button::setWndMenu()
{
    HMENU* hmenu = getWndMenu();
    *hmenu = (HMENU)ID_OUTPUT;
}

LRESULT CALLBACK outputControlProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return 0;
}

WNDPROC output_button::getWndProc()
{
    return outputControlProc;
}
