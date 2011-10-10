#include "Window.h"

Window * Window::window = NULL;
IrrlichtDevice *Window::device = NULL;

Window::Window()
{
}

bool Window::beginWindowsRender()
{
    //commence le rendu de la scene
    run=device->run();
    driver->beginScene(true, true, SColor(255,0,255,255));
    // display frames per second in window title
    int fps = driver->getFPS();
    int lastFPS=0;
    if (lastFPS != fps)
    {
        core::stringw str = L"test ";
        str += " FPS:";
        str += fps;

        device->setWindowCaption(str.c_str());
        lastFPS = fps;
    }
    return run;
}

bool Window::endWindowsRender()
{
    //fini le rendu de la scene
    driver->endScene();
    return run;
}

IrrlichtDevice * Window::getDevice()
{
    return device;
}

Window::~Window()
{
    device->drop();
}
