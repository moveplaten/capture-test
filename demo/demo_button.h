#pragma once

#include <Windows.h>


enum
{
    ID_SETTING = 5000,
    ID_PREVIEW,
    ID_OUTPUT,
    ID_REGION,
};


class _button
{
private:
    /* data */
    int width;
    int height;

    DWORD initial_style;

public:
    int getWidth() { return width; };
    int getHeight() { return height; };
    DWORD getInitStyle() { return initial_style; };

    _button();
    ~_button();
};


class demo_button : public _button
{
private:
    /* data */
    char* btnName;
    HWND hwndSelf;
    bool isClicked;
    char* wndClass;
    HMENU hwndMenu;
    HINSTANCE hinstMain;

public:
    virtual void setBtnName() {};
    void setHwndSelf(HWND hwnd);
    void reverseClickStat();
    virtual void setWndClass() {};
    virtual void setWndMenu() {};
    void setHinstMain();
    
    char* getBtnName() { return btnName; };
    HWND getHwndSelf() { return hwndSelf; };
    bool getClickStat() { return isClicked; };
    char* getWndClass() { return wndClass; };
    HMENU* getWndMenu() { return &hwndMenu; };
    virtual WNDPROC getWndProc() = 0;
    HINSTANCE getInstance() { return hinstMain; };

    bool initControl();
    void internInit(char* _btnName, char* _wndClass, HMENU _hwndMenu);
    bool createWndSelf(HWND hwndMain);
    //virtual LRESULT CALLBACK wndControlProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    demo_button();
    ~demo_button();
};



/* Below Real Button*/

class real_button : public demo_button
{
private:
    /* data */
    WNDPROC wndProc;

public:
    virtual WNDPROC getWndProc(){ return wndProc; };

    real_button(char* _btnName, char* _wndClass, HMENU _hwndMenu)
    {
        wndProc = nullptr;
        internInit(_btnName, _wndClass, _hwndMenu);
    };

    real_button(char* _btnName, char* _wndClass, HMENU _hwndMenu, WNDPROC _wndProc)
    {
        wndProc = _wndProc;
        internInit(_btnName, _wndClass, _hwndMenu);
    };

    ~real_button() {};
};
