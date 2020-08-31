#include <windows.h>

void * CaptureInternal(void);

static void capture_DC(HWND hwnd);

int capture(HWND hwnd)
{
//    HDC hdcFullScreen;
    HDC hdcWindow;
    RECT rcClient;

//    hdcFullScreen = GetDC(NULL);
    hdcWindow = GetDC(hwnd);

    GetClientRect(hwnd, &rcClient);

    SetStretchBltMode(hdcWindow, HALFTONE); //Mode

//    HDC hdcMem = CreateCompatibleDC(hdcWindow);
    void * mempp = CaptureInternal(); //TODO
    if(mempp == NULL)
    {
        capture_DC(hwnd);
    }
//    HBITMAP hBitmap = CreateBitmap(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 1, 32, mempp);
//    free(mempp);
//    HGDIOBJ Old = SelectObject(hdcMem, hBitmap);

    BITMAPINFO bi;

    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth = GetSystemMetrics(SM_CXSCREEN);
    bi.bmiHeader.biHeight = - GetSystemMetrics(SM_CYSCREEN);
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    bi.bmiHeader.biSizeImage = 0;
    bi.bmiHeader.biXPelsPerMeter = 0;
    bi.bmiHeader.biYPelsPerMeter = 0;
    bi.bmiHeader.biClrUsed = 0;
    bi.bmiHeader.biClrImportant = 0;

    StretchDIBits(hdcWindow,
        0, 0,
        rcClient.right, rcClient.bottom,
        0, 0,
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
        mempp,
        &bi,
        DIB_RGB_COLORS,
        SRCCOPY);
//    free(mempp);
//    SelectObject(hdcMem, Old);
//    DeleteObject(hBitmap);
//    ReleaseDC(hwnd, hdcMem);
    ReleaseDC(hwnd, hdcWindow);

    return 0;
}

static void capture_DC(HWND hwnd)
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
}