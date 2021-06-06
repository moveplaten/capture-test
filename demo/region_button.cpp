#include "demo_button.h"

LRESULT CALLBACK regionControlProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

real_button regionBtn("REGION", "regionClass", (HMENU)ID_REGION, regionControlProc);


LRESULT CALLBACK regionControlProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return 0;
}
