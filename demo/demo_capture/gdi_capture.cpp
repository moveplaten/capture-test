#include "gdi_capture.h"
#include <stdio.h>

int gdi_capture::internInit(HWND _hwndDst)
{
    hwndDst = _hwndDst;
    strcat(getName(), "GDI");
    return 0;
}

int gdi_capture::captureLoop()
{
    HDC hdcFullScreen;
    HDC hdcWindow;
    RECT rcClient;

    hdcFullScreen = GetDC(NULL);
    hdcWindow = GetDC(hwndDst);

    GetClientRect(hwndDst, &rcClient);

    SetStretchBltMode(hdcWindow, HALFTONE); //Mode

    bool OK = StretchBlt(hdcWindow,
        0, 0,
        rcClient.right, rcClient.bottom,
        hdcFullScreen,
        0, 0,
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
        SRCCOPY);

    ReleaseDC(NULL, hdcFullScreen);
    ReleaseDC(hwndDst, hdcWindow);

    return 0;
}


gdi_capture::gdi_capture()
{
    hwndDst = nullptr;
}

gdi_capture::~gdi_capture()
{
}
