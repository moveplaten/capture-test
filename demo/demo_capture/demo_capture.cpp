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
            dc->setFPS(dc->fpsCount());
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

static inline LONGLONG getQPCInterval(void)
{
    LARGE_INTEGER QPC;
    bool clearStamp1 = false;
    static LONGLONG stamp[2] = { 0, 0 };

    if (!stamp[0])
    {
        QueryPerformanceCounter(&QPC);
        stamp[0] = QPC.QuadPart;
        clearStamp1 = true;
    }
    else
    {
        QueryPerformanceCounter(&QPC);
        stamp[1] = QPC.QuadPart;
    }

    LONGLONG QPCInterval = abs(stamp[0] - stamp[1]);

    if (clearStamp1) stamp[1] = 0;
    else stamp[0] = 0;

    return QPCInterval;
}

double demo_capture::fpsCount(void)
{
    LARGE_INTEGER Frequency;
    QueryPerformanceFrequency(&Frequency);
    double timeInterval = (double)getQPCInterval() / (double)Frequency.QuadPart;
    double fps = (double)1.0 / timeInterval;
    return fps;
}