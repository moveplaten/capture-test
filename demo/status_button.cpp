#include <stdio.h>
#include "demo_button.h"
#include "demo_capture/gdi_capture.h"

extern gdi_capture gdiCapt;

LRESULT CALLBACK statusControlProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

real_button statusBtn("STATUS", "statusClass", (HMENU)ID_STATUS, statusControlProc);

void textDisplay(HDC hdc, HWND hwnd)
{
    RECT rect;
    GetClientRect(hwnd, &rect);
    FillRect(hdc, &rect, (HBRUSH)COLOR_WINDOW - 0);

    SetTextColor(hdc, RGB(0, 100, 200));
    SetBkColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);
    char szText[200];
    sprintf(szText, " FPS : %f | Method : %s | %s", gdiCapt.getFPS(), gdiCapt.getName(), gdiCapt.getRunning() ? "Running" : "Stop");
    TextOut(hdc, 0, 0, szText, strlen(szText));
}

LRESULT CALLBACK statusControlProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        static bool first = true;
        if (first)
        {
            SetTimer(hwnd, 1, 1000, nullptr);
            first = false;
        }
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        
        textDisplay(hdc, hwnd);
        
        EndPaint(hwnd, &ps);
    }
    break;

    case WM_TIMER:
    {
        HDC hdc = GetDC(hwnd);

        textDisplay(hdc, hwnd);

        ReleaseDC(hwnd, hdc);
    }

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}
