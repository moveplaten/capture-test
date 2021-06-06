#include "demo_button.h"

LRESULT CALLBACK previewControlProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

real_button previewBtn("PREVIEW", "previewClass", (HMENU)ID_PREVIEW, previewControlProc);


LRESULT CALLBACK previewControlProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return 0;
}
