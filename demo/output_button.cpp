#include "demo_button.h"

LRESULT CALLBACK outputControlProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

real_button outputBtn("OUTPUT", "outputClass", (HMENU)ID_OUTPUT, outputControlProc);


LRESULT CALLBACK outputControlProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return 0;
}
