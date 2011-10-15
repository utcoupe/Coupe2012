#ifndef WINDOWSMANAGER
#define WINDOWSMANAGER

#include "main.h"
#include IRRLICHTPATH
#include "Input.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace std;
using namespace core;
using namespace io;

//gere la fenetre principale avec irrlicht
class Window
{
private :
    static Window* window;
    Window();
protected:
    static IrrlichtDevice *device;
    IVideoDriver* driver;
    bool run ;

public:
    ~Window();
    static Window * getWindow(){
        if(window==NULL){
            window=new Window;
            window->run=true;
            //crée la fenetre
            window->device = createDevice(irr::video::EDT_OPENGL, dimension2d<u32>(LARGEUR_ECRAN,HAUTEUR_ECRAN), 16,
                                  false, false, true, 0);
            window->driver = window->device->getVideoDriver();
            device->setEventReceiver(EventManager::getEventManager());
        }
        return window;
    }
    bool beginWindowsRender();
    bool endWindowsRender();
    IrrlichtDevice * getDevice();
    static ISceneManager * getSmgr(){
        return device->getSceneManager();
    }
};

#endif
