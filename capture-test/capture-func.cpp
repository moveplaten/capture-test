#include <windows.h>

void * CaptureInternal(void);

int capture(HWND hwnd)
{
//    HDC hdcFullScreen;
    HDC hdcWindow;
    RECT rcClient;

//    hdcFullScreen = GetDC(NULL);
    hdcWindow = GetDC(hwnd);

    GetClientRect(hwnd, &rcClient);

    SetStretchBltMode(hdcWindow, HALFTONE); //Mode

    HDC hdcMem = CreateCompatibleDC(hdcWindow);
    void * mempp = CaptureInternal(); //TODO
    HBITMAP hBitmap = CreateBitmap(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 1, 32, mempp);
    free(mempp);
    HGDIOBJ Old = SelectObject(hdcMem, hBitmap);

    StretchBlt(hdcWindow,
        0, 0,
        rcClient.right, rcClient.bottom,
        hdcMem,
        0, 0,
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
        SRCCOPY);

    SelectObject(hdcMem, Old);
    DeleteObject(hBitmap);
    ReleaseDC(hwnd, hdcMem);
    ReleaseDC(hwnd, hdcWindow);

    return 0;
}