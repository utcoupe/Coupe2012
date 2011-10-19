#ifndef PHYSIC_H
#define PHYSIC_H

#include "btBulletDynamicsCommon.h"
#include <list>

class Physic
{
    Physic(){}
    public:

    static std::list<btRigidBody *> objects;
    static btDiscreteDynamicsWorld *world;

        static void init(){
            // Initialize bullet
            btDefaultCollisionConfiguration *CollisionConfiguration = new btDefaultCollisionConfiguration();
            btBroadphaseInterface *BroadPhase = new btAxisSweep3(btVector3(-1000, -1000, -1000), btVector3(1000, 1000, 1000));
            btCollisionDispatcher *Dispatcher = new btCollisionDispatcher(CollisionConfiguration);
            btSequentialImpulseConstraintSolver *Solver = new btSequentialImpulseConstraintSolver();
            world = new btDiscreteDynamicsWorld(Dispatcher, BroadPhase, Solver, CollisionConfiguration);
        }

        ~Physic();
    protected:
    private:
};

#endif // PHYSIC_H
