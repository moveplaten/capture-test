#include "demo_button.h"

LRESULT CALLBACK settingControlProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

real_button settingBtn("SETTING", "settingClass", (HMENU)ID_SETTING, settingControlProc);


LRESULT CALLBACK settingControlProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return 0;
}
