#include <Windows.h>
#include <CommCtrl.h>
#include "demo_button.h"
#include "demo_capture/gdi_capture.h"


#pragma comment(linker,"\"/manifestdependency:type='win32' "\
                        "name='Microsoft.Windows.Common-Controls' "\
                        "version='6.0.0.0' processorArchitecture='amd64' "\
                        "publicKeyToken='6595b64144ccf1df' language='*'\"")

#define MAINWNDCLASS TEXT("MainWindow")
#define MIN(a, b)        (((a) < (b)) ? (a) : (b))
#define MAX(a, b)        (((a) > (b)) ? (a) : (b))

/*GLOBAL */
int borderXSize, borderYSize;
extern real_button settingBtn, previewBtn, outputBtn, regionBtn;
gdi_capture gdiCapt;


LRESULT CALLBACK MainProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void windowResize(HWND hwnd);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = MAINWNDCLASS;
    wc.lpfnWndProc = (WNDPROC)MainProc;
    wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    if (!RegisterClass(&wc)) exit(1);


    int x, y, width, height;
    x = y = 0;
    width = height = 600;
    HWND hwndMain = CreateWindowEx(WS_EX_CONTROLPARENT | WS_EX_WINDOWEDGE | WS_EX_LEFT, MAINWNDCLASS, TEXT("demo"),
        WS_OVERLAPPED | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN,
        x, y, width, height, NULL, NULL, hInstance, NULL);

    settingBtn.createWndSelf(hwndMain);
    previewBtn.createWndSelf(hwndMain);
    outputBtn.createWndSelf(hwndMain);

    regionBtn.createWndSelf(hwndMain);
    void* originWndProc;
    originWndProc = (void*) GetWindowLongPtrW(regionBtn.getHwndSelf(), GWLP_WNDPROC);
    SetWindowLongPtr(regionBtn.getHwndSelf(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(regionBtn.getWndProc()));

    borderXSize = borderYSize = 0;
    borderXSize += (GetSystemMetrics(SM_CXSIZEFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER)) * 2;
    borderYSize += (GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER)) * 2;
    borderYSize += GetSystemMetrics(SM_CYCAPTION);

    windowResize(hwndMain);

    ShowWindow(hwndMain, SW_SHOW);
    UpdateWindow(hwndMain);

    //gdiCapt.internInit(regionBtn.getHwndSelf());
    gdiCapt.captureThreadStart(regionBtn.getHwndSelf());
    SetWindowTextA(previewBtn.getHwndSelf(), "STOP");
    previewBtn.reverseClickStat();

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!IsDialogMessage(hwndMain, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK MainProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_SETTING:
        {
            MessageBox(hwnd, "OK1", 0, 0);
        }
        break;

        case ID_PREVIEW:
        {
            if (!previewBtn.getClickStat())
            {
                gdiCapt.captureThreadRestart();
                previewBtn.reverseClickStat();
                SetWindowTextA(previewBtn.getHwndSelf(), "STOP");
            }
            else
            {
                gdiCapt.captureThreadStop();
                previewBtn.reverseClickStat();
                SetWindowTextA(previewBtn.getHwndSelf(), previewBtn.getBtnName());
            }
        }
        break;

        case ID_OUTPUT:
        {
            MessageBox(hwnd, "OK3", 0, 0);
        }
        break;

        case ID_REGION:
        {
            MessageBox(hwnd, "OK4", 0, 0);
        }
        break;

        }
    break;

    case WM_SIZING:
    {
        RECT& screenSize = *(RECT*)lParam;

        int newWidth = MAX(screenSize.right - screenSize.left, 480 + borderXSize);
        int newHeight = MAX(screenSize.bottom - screenSize.top, 330 + borderYSize);

        if (wParam == WMSZ_LEFT || wParam == WMSZ_BOTTOMLEFT || wParam == WMSZ_TOPLEFT)
            screenSize.left = screenSize.right - newWidth;
        else
            screenSize.right = screenSize.left + newWidth;

        if (wParam == WMSZ_BOTTOM || wParam == WMSZ_BOTTOMLEFT || wParam == WMSZ_BOTTOMRIGHT)
            screenSize.bottom = screenSize.top + newHeight;
        else
            screenSize.top = screenSize.bottom - newHeight;

        return TRUE;
    }

    case WM_SIZE:
    {
        windowResize(hwnd);
    }
    break;

    case WM_CLOSE:
    {
        PostQuitMessage(0);
    }
    break;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

void windowResize(HWND hwnd)
{
    int ID_BtnSpace = 70;
    int Btn_StartX, Btn_StartY;

    RECT client;
    GetClientRect(hwnd, &client);

    Btn_StartX = (client.right - 3 * settingBtn.getWidth() - 2 * ID_BtnSpace) / 2;

    int top_margin = 70;
    int RgnWidth = client.right;
    int RgnHeight = client.bottom - top_margin;
    //int ViewRgnCenterX = RgnWidth / 2;
    //int ViewRgnCenterY = top_margin + RgnHeight / 2;

    int ViewRgnStartX, ViewRgnStartY, ViewRgnWidth, ViewRgnHeight = 0;
    if ((float)RgnWidth / (float)RgnHeight >= (float)GetSystemMetrics(SM_CXSCREEN) / (float)GetSystemMetrics(SM_CYSCREEN)) // 16 : 9
    {
        ViewRgnHeight = RgnHeight;
        ViewRgnWidth = ViewRgnHeight * ((float)GetSystemMetrics(SM_CXSCREEN) / (float)GetSystemMetrics(SM_CYSCREEN));

        ViewRgnStartX = (RgnWidth - ViewRgnWidth) / 2;
        ViewRgnStartY = top_margin;
    }
    else
    {
        ViewRgnWidth = RgnWidth;
        ViewRgnHeight = ViewRgnWidth * ((float)GetSystemMetrics(SM_CYSCREEN) / (float)GetSystemMetrics(SM_CXSCREEN));

        ViewRgnStartX = 0;
        ViewRgnStartY = top_margin + (RgnHeight - ViewRgnHeight) / 2;
    }

    Btn_StartY = 20;
    DWORD flags = SWP_NOOWNERZORDER | SWP_SHOWWINDOW;

    SetWindowPos(settingBtn.getHwndSelf(), NULL, Btn_StartX, Btn_StartY, settingBtn.getWidth(), settingBtn.getHeight(), flags);
    Btn_StartX += settingBtn.getWidth() + ID_BtnSpace;

    SetWindowPos(previewBtn.getHwndSelf(), NULL, Btn_StartX, Btn_StartY, previewBtn.getWidth(), previewBtn.getHeight(), flags);
    Btn_StartX += previewBtn.getWidth() + ID_BtnSpace;

    SetWindowPos(outputBtn.getHwndSelf(), NULL, Btn_StartX, Btn_StartY, outputBtn.getWidth(), outputBtn.getHeight(), flags);

    SetWindowPos(regionBtn.getHwndSelf(), NULL, ViewRgnStartX, ViewRgnStartY, ViewRgnWidth, ViewRgnHeight, flags);
}
