#include "Map.h"

Map * Map::map = NULL;

bool Map::run() {
    deltaTime = irrTimer->getTime() - timeStamp;
    timeStamp = irrTimer->getTime();
    UpdatePhysics(deltaTime);
    if(EventManager::getEventManager()->getEvent(KEY_F1)==true){
        camera->remove();
        camera=smgr->addCameraSceneNodeFPS(0,30,0.003);
        camera->setName("fps");
        camera->setPosition(vector3df(0,3,1));
        camera->setTarget(vector3df(0,0,0));
    }
    else if(EventManager::getEventManager()->getEvent(KEY_F2)==true){
        camera->remove();
        camera=smgr->addCameraSceneNode();
        camera->setName("track");
        camera->setPosition(vector3df(0,3,1));
        camera->setTarget(vector3df(0,0,0));
    }
    smgr->drawAll();
    return true;
}
void Map::init() {
    IrrlichtDevice * device = Window::getWindow()->getDevice();
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();

    skydome=smgr->addSkyDomeSceneNode(driver->getTexture("media/skydome.jpg"),16,8,0.95f,2.0f);


    //lumiere ambiante
    smgr->setAmbientLight (video::SColorf(1.0,1.0,1.0,1.0));
    //lumiere diffuse
    smgr->addLightSceneNode (0, core::vector3df(0,3,0), video::SColorf(1.0f,1.0f,1.0f,1.0f), 4.0f);
    //smgr->addLightSceneNode (0, core::vector3df(0,0,50), video::SColorf(1.0f,1.0f,1.0f,1.0f), 100.0f);
    //smgr->addLightSceneNode (0, core::vector3df(50,0,0), video::SColorf(1.0f,1.0f,1.0f,1.0f), 100.0f);

    //camera->remove();
    camera=smgr->addCameraSceneNode();
    camera->setPosition(vector3df(0,2,0.1));
    camera->setTarget(vector3df(0,0,0));
    camera->setName("track");
    //camera->setPosition(vector3df(0,40,40));

    Physic::init();

    irrTimer = device->getTimer();
    timeStamp = irrTimer->getTime();
    deltaTime = 0;
}

Map::~Map() {

}

bool Map::load(std::string filename) {
    smgr->addOctreeSceneNode(smgr->getMesh(filename.c_str()),0,-1,1);
    return true;
}
