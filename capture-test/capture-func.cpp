#include <windows.h>

int capture(HWND hwnd)
{
    HDC hdcFullScreen;
    HDC hdcWindow;
    RECT rcClient;

    hdcFullScreen = GetDC(NULL);
    hdcWindow = GetDC(hwnd);

    GetClientRect(hwnd, &rcClient);

    SetStretchBltMode(hdcWindow, HALFTONE); //Mode

    StretchBlt(hdcWindow,
        0, 0,
        rcClient.right, rcClient.bottom,
        hdcFullScreen,
        0, 0,
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
        SRCCOPY);

    ReleaseDC(NULL, hdcFullScreen);
    ReleaseDC(hwnd, hdcWindow);

    return 0;
}