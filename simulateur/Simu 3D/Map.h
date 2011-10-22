#ifndef MAP_MANAGER
#define MAP_MANAGER

#include "main.h"
#include "Window.h"
#include "irrlicht.h"
#include "Physic.h"
#include <list>

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
    irr::ITimer *irrTimer;
	u32 timeStamp, deltaTime;

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

    void UpdatePhysics(u32 TDeltaTime) {
        Physic::world->stepSimulation(TDeltaTime * 0.001f, 60);

        std::list<btRigidBody *>::iterator iterator;
        // Relay the object's orientation to irrlicht
        for(iterator = Physic::objects.begin(); iterator != Physic::objects.end(); ++iterator) {
            UpdateRender(*iterator);
        }
    }

    // Passes bullet's orientation to irrlicht
    void UpdateRender(btRigidBody *TObject) {
        ISceneNode *Node = static_cast<ISceneNode *>(TObject->getUserPointer());

        // Set position
        btVector3 Point = TObject->getCenterOfMassPosition();
        Node->setPosition(vector3df((f32)Point[0], (f32)Point[1], (f32)Point[2]));

        // Set rotation
        vector3df Euler;
        const btQuaternion& TQuat = TObject->getOrientation();
        quaternion q(TQuat.getX(), TQuat.getY(), TQuat.getZ(), TQuat.getW());
        q.toEuler(Euler);
        Euler *= RADTODEG;
        Node->setRotation(Euler);
    }

    ~Map();
    bool load(std::string);
    bool run();
};

#endif
