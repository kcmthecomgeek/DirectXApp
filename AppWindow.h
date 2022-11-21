#pragma once
#include "Window.h"


class AppWindow :
    public Window
{
public:
    AppWindow();
    ~AppWindow();

    // Inherited
    virtual void onCreate() override;
    virtual void onUpdate() override;
    virtual void onDestroy() override;


};

