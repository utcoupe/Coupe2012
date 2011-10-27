#ifndef Robot_H
#define Robot_H

#define GK_RESOURCE_BUGGY_FILE    "media/robot.blend"
#define GK_RESOURCE_BUGGY_GROUP   "CarGroup"
#define GK_RESOURCE_BUGGY_PHYSOBJ "ChassisCollision"
#define GK_RESOURCE_BUGGY_CHASSIS "Chassis"
#define GK_RESOURCE_BUGGY_WHELLFL "TireFL"
#define GK_RESOURCE_BUGGY_WHELLFR "TireFR"
#define VEHICLE_RESOURCE_GROUP "VehicleDemo"

#include "OgreKit.h"
#include "btBulletDynamicsCommon.h"
#include "Asserv.h"

typedef struct gkWheelProperties
{
	gkGameObject* m_object;
	gkScalar      m_radius;
	bool          m_isFront;
	gkVector3     m_connectionPoint;
	gkVector3     m_wheelDirection;
	gkVector3     m_wheelAxle;
	gkScalar      m_restLength;
	gkScalar      m_stiffness;
	gkScalar      m_dampingRelax;
	gkScalar      m_dampingComp;
	gkScalar      m_friction;
	gkScalar      m_rollInfluence;
	gkScalar      m_travelDistCm;

} gkWheelProperties;

class Robot : public gkDynamicsWorld::Listener
{
private:
    Asserv asserv;
	gkScene*                           m_scene;
	gkDynamicsWorld*                   m_dynamicWorld;
	btDefaultVehicleRaycaster*         m_raycaster;
	btRaycastVehicle*                  m_vehicle;
	btRaycastVehicle::btVehicleTuning  m_tuning;
	gkGameObject*                      m_object;
	utArray<gkWheelProperties*>        m_wheels;
	btRigidBody*                       m_chassis;

	gkScalar m_engineTorque;
	gkScalar m_currentRpm;

	gkScalar m_gazR;
	gkScalar m_gazL;

	void updateTransmition(gkScalar rate);
	void updateWheels(gkScalar rate);
	void load();
	void createVehicle(void);

    void addWheel(gkGameObject* object, gkScalar radius, gkVector3 connectionPoint, gkVector3 wheelDirection,
	              gkVector3 wheelAxle, bool isFront, gkScalar restLength, gkScalar stiffness, gkScalar dampingRelax,
	              gkScalar dampingComp, gkScalar friction, gkScalar roll, gkScalar travelDist);

	void setTransform(const gkTransformState& v) { m_object->setTransform(v); }

	void setEngineTorque(gkScalar v)             { m_engineTorque = v; }
	void setChassisObject(gkGameObject* v)       { m_object = v; }
	void subtick(gkScalar rate);
	void presubtick(gkScalar rate);
	void updateVehicle(gkScalar rate);

	float mmx2float(int mmx){
        return (mmx/1000.0)-1.5;
	}

    float mmy2float(int mmy){
        return -((mmy/1000.0)-1.0);
	}

public:

	Robot()
	: m_dynamicWorld(0), m_raycaster(0), m_vehicle(0), m_object(0), m_chassis(0),
	  m_gazR(0), m_gazL(0), m_engineTorque(0), m_currentRpm(0)
    {
    }
	~Robot();


	void setup(gkScene * scene,int posx,int posy,double angle){
	    m_scene=scene;
        load();
        createVehicle();
        // Move to start line
        //setTransform(gkTransformState(gkVector3(mmx2float(posx),mmy2float(posy),0), gkEuler(0, 0, angle).toQuaternion()));

	    asserv.init();
	    //asserv.turn(-90);
	    asserv.goTo(1000,1000);
	}

	void tick(gkScalar rate);

	void setGazR(gkScalar ratio)                  { m_gazR = ratio; }
	void setGazL(gkScalar ratio)                  { m_gazL = ratio; }
	void goTo(int x,int y){
        asserv.goTo(x,y);
	}

	gkScalar getCurrentSpeedKmHour(void)         { return m_vehicle->getCurrentSpeedKmHour(); }
	gkScalar getVelocityEulerZ(void);
	gkScalar getCurrentRpm(void)                 { return m_currentRpm; }

};

#endif // Robot_H
