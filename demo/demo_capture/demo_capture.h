#pragma once

#include <Windows.h>

class demo_capture
{
private:
    /* data */
    char* name;
    double fps;
    bool running;
    HANDLE stopEvent;
    HANDLE startEvent;
    DWORD ThreadId;

public:
    bool captureThreadStart();  //just call once, then call Restart();
    virtual int captureLoop() = 0;
    void captureThreadStop();
    void captureThreadRestart();

    virtual int internInit() = 0;

    void setFPS(double _fps) { fps = _fps; }

    char* getName() { return name; }
    double getFPS() { return fps; }
    bool getRunning() { return running; }
    HANDLE getStopEvent() { return stopEvent; }
    HANDLE getStartEvent() { return startEvent; }
    DWORD getThreadId() { return ThreadId; }


    demo_capture();
    ~demo_capture();
};
