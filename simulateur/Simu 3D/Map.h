#ifndef MAP_MANAGER
#define MAP_MANAGER

#include "main.h"
#include "Window.h"
#include IRRLICHTPATH

using namespace irr;
using namespace scene;
using namespace video;
using namespace std;
using namespace core;
using namespace io;

class Map
{
private:

    static Map * map;
    IrrlichtDevice *device ;
    IVideoDriver *driver ;
    ISceneManager *smgr ;
    ISceneNode *skydome;
    ICameraSceneNode* camera;

    Map(){}
    void init();

public:
    static Map* getMap(){
        if(map==NULL){
            map=new Map;
            map->init();
        }
        return map;
    }
    ~Map();
    bool load(std::string);
    bool run();
};

#endif
