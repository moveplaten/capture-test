#include "demo_capture.h"

DWORD WINAPI captureThread(_In_ void* param);

bool demo_capture::captureThreadStart(HWND _hwndDst)
{
    internInit(_hwndDst);
    stopEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    startEvent = CreateEvent(nullptr, TRUE, TRUE, nullptr);
    CreateThread(nullptr, 0, captureThread, this, 0, &ThreadId);

    running = true;

    return true;
}

DWORD captureThread(_In_ void* param)
{
    demo_capture* dc = (demo_capture*)param;

    for (;;)
    {
        if (WaitForSingleObjectEx(dc->getStopEvent(), 0, FALSE) == WAIT_OBJECT_0)
        {
            WaitForSingleObjectEx(dc->getStartEvent(), INFINITE, FALSE);
        }
        else
        {
            dc->captureLoop();
        }
    }

    return 0;
}

void demo_capture::captureThreadStop()
{
    ResetEvent(startEvent);
    SetEvent(stopEvent);
    running = false;
}

void demo_capture::captureThreadRestart()
{
    ResetEvent(stopEvent);
    SetEvent(startEvent);
    running = true;
}

demo_capture::demo_capture()
{
    name = (char*)malloc(20);
    ZeroMemory(name, 20);
    fps = 0.0f;
    running = false;
    stopEvent = nullptr;
    startEvent = nullptr;
    ThreadId = 0;
}

demo_capture::~demo_capture()
{
}
