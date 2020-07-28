#include <stdio.h>
#include <windows.h>
extern bool moving;

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

int capture(HWND hwnd);

typedef struct CapRect {
    int x, y, width, height;
} CapRect;


int main(void)
{
    printf("capture test!\n");
    moving = FALSE;
    HINSTANCE hInstance = GetModuleHandle(0);
    int nCmdShow = SW_SHOWDEFAULT;

    MyRegisterClass(hInstance);
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        printf("\ndestroying window\n");
        PostQuitMessage(0);
        break;

    case WM_LBUTTONDOWN:
    {
        printf("\nmouse left button down at (%d, %d)\n", LOWORD(lParam), HIWORD(lParam));
        //ValidateRect(hWnd, NULL);
        //PAINTSTRUCT ps;
        moving = FALSE;
        HDC hdc = GetDC(hWnd);

        SetTextColor(hdc, RGB(0, 255, 255));
        SetBkColor(hdc, RGB(0, 0, 0));
        SetBkMode(hdc, TRANSPARENT);
        char szText[256] = "....................Testing";
        TextOut(hdc, 0, 0, szText, strlen(szText));

        ReleaseDC(hWnd, hdc);


        ////////////////////////////
        capture(hWnd);
        ////////////////////////////


        hdc = GetDC(hWnd);
        SetTextColor(hdc, RGB(255, 0, 0));
        SetBkColor(hdc, RGB(0, 0, 0));
        SetBkMode(hdc, TRANSPARENT);
        char szText2[256] = "Click Again";
        TextOut(hdc, 0, 0, szText2, strlen(szText2));

        ReleaseDC(hWnd, hdc);
    }
    break;


    case WM_MOVING:
    {
        moving = TRUE;
        ////////////////////////////
        capture(hWnd);
        ////////////////////////////
    }
    break;


    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        SetTextColor(hdc, RGB(255, 0, 0));
        SetBkColor(hdc, RGB(0, 0, 0));
        SetBkMode(hdc, TRANSPARENT);

        char szText[256] = "Click";
        TextOutA(hdc, 0, 0, szText, strlen(szText));

        EndPaint(hWnd, &ps);
    }
    break;

    default:
        printf(".");
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


const char* szWindowClass = "mainClass";

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    return RegisterClassEx(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    CapRect rect;
    rect.x = 300;
    rect.y = 300;
    rect.width = 640;
    rect.height = 480;

    HWND hWnd = CreateWindowEx(0, szWindowClass, "capture-test",
        WS_THICKFRAME,
        rect.x, rect.y, rect.width, rect.height, 0, 0, hInstance, 0);

    if (!hWnd)
    {
        return FALSE;
    }

    SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}
