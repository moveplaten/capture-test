#pragma once

#include "demo_capture.h"

class gdi_capture : public demo_capture
{
private:
    /* data */
    HWND hwndDst;

protected:
    virtual int captureLoop();
    virtual int internInit(HWND _hwndDst);

public:

    
    gdi_capture();
    ~gdi_capture();
};
