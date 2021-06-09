#pragma once

#include "demo_capture.h"

class gdi_capture : public demo_capture
{
private:
    /* data */
    HWND hwndDst;

public:
    virtual int captureLoop();
    virtual int internInit() { return 0; }
    virtual int internInit(HWND hwnd);

    gdi_capture();
    ~gdi_capture();
};
