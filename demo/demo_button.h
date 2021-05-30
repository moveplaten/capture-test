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
    virtual void setBtnName() = 0;
    void setHwndSelf(HWND hwnd);
    void reverseClickStat();
    virtual void setWndClass() = 0;
    virtual void setWndMenu() = 0;
    void setHinstMain();
    
    char* getBtnName() { return btnName; };
    HWND getHwndSelf() { return hwndSelf; };
    bool getClickStat() { return isClicked; };
    char* getWndClass() { return wndClass; };
    HMENU* getWndMenu() { return &hwndMenu; };
    virtual WNDPROC getWndProc() = 0;
    HINSTANCE getInstance() { return hinstMain; };

    bool initControl();
    void internInit();
    bool createWndSelf(HWND hwndMain);
    //virtual LRESULT CALLBACK wndControlProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    demo_button();
    ~demo_button();
};



/* Below 3 + 1 Button*/

class setting_button : public demo_button
{
private:
    /* data */
public:
    virtual void setBtnName();
    virtual void setWndClass();
    virtual void setWndMenu();
    virtual WNDPROC getWndProc();

    setting_button();
    ~setting_button();
};

class preview_button : public demo_button
{
private:
    /* data */
public:
    virtual void setBtnName();
    virtual void setWndClass();
    virtual void setWndMenu();
    virtual WNDPROC getWndProc();

    preview_button();
    ~preview_button();
};

class output_button : public demo_button
{
private:
    /* data */
public:
    virtual void setBtnName();
    virtual void setWndClass();
    virtual void setWndMenu();
    virtual WNDPROC getWndProc();

    output_button();
    ~output_button();
};

class region_button : public demo_button
{
private:
    /* data */
public:
    virtual void setBtnName();
    virtual void setWndClass();
    virtual void setWndMenu();
    virtual WNDPROC getWndProc();

    region_button();
    ~region_button();
};
