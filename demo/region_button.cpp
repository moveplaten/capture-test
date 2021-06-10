#include "demo_button.h"

LRESULT CALLBACK regionControlProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

real_button regionBtn("REGION", "regionClass", (HMENU)ID_REGION, regionControlProc);


LRESULT CALLBACK regionControlProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
    {
        HDC hdc = GetDC(hwnd);

        SetTextColor(hdc, RGB(0, 255, 255));
        SetBkColor(hdc, RGB(0, 0, 0));
        SetBkMode(hdc, TRANSPARENT);
        char szText[256] = "....................Testing";
        TextOut(hdc, 0, 0, szText, strlen(szText));

        ReleaseDC(hwnd, hdc);
    }
    break;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}
