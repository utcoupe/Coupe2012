#include "Robot.h"

void Robot::load(void)
{
	gkBlendFile* pBlendFileCar = gkBlendLoader::getSingleton().loadFile(gkUtils::getFile(GK_RESOURCE_BUGGY_FILE), "", VEHICLE_RESOURCE_GROUP);

	gkScene* carScene = pBlendFileCar->getMainScene();
	gkSceneManager::getSingleton().copyObjects(carScene, m_scene);
	gkGameObject* objCol = m_scene->getObject(GK_RESOURCE_BUGGY_PHYSOBJ);

#ifdef _DEBUG
	gkGameObject* objChassis = m_scene->getObject(GK_RESOURCE_BUGGY_CHASSIS);
	gkGameObject* o = m_scene->getObject("Circle.022");
	GK_ASSERT(o && o->getParent() && o->getParent() == objChassis);
#endif

	// general
	setChassisObject(objCol);
	setEngineTorque(330);

	double scale = 0.2;

	// wheels
	float connectionHeight = 0.267*scale;
	/*float sideOffsetF = 0.764;
	float sideOffsetR = 0.848;
	float frontOffest = 0.988;
	float rearOffset = 0.965;*/
    float sideOffsetF = 0.764*scale;
	float sideOffsetR = 0.848*scale;
	float frontOffest = 0;
	float rearOffset = 0;
	float radius = 0.370*scale;

	float suspensionTravelCm = 40*scale;
	float suspensionRestLength = .5*scale;
	float suspensionStiffness = 22.0;
	float suspensionRelax = 0.6 * 2.0 * btSqrt(suspensionStiffness);
	float suspensionCompression = 0.6 * 2.0 * btSqrt(suspensionStiffness);
	float friction = 2.0f;
	float rollInfluence = 0.1;

	gkVector3 wheelDirection(0, 0, -1);
	gkVector3 wheelAxle(1, 0, 0);
	gkVector3 connectionPoint(0, 0, 0);

	bool isFront = true;
	gkGameObject* wheel;

	wheel = m_scene->getObject(GK_RESOURCE_BUGGY_WHELLFL);
	connectionPoint = gkVector3(-sideOffsetF, frontOffest, connectionHeight);
	addWheel(wheel, radius, connectionPoint, wheelDirection, wheelAxle, isFront,
	                    suspensionRestLength, suspensionStiffness, suspensionRelax, suspensionCompression,
	                    friction, rollInfluence, suspensionTravelCm);

	wheel = m_scene->getObject(GK_RESOURCE_BUGGY_WHELLFR);
	connectionPoint = gkVector3(sideOffsetF, frontOffest, connectionHeight);
	addWheel(wheel, radius, connectionPoint, wheelDirection, wheelAxle, isFront,
	                    suspensionRestLength, suspensionStiffness, suspensionRelax, suspensionCompression,
	                    friction, rollInfluence, suspensionTravelCm);
}

Robot::~Robot()
{
	if(m_dynamicWorld)
		m_dynamicWorld->removeListener(this);

	gkWheelProperties** ptr = m_wheels.ptr();
	for (UTsize i = 0; i < m_wheels.size(); ++i)
		delete ptr[i];

	if(m_vehicle)
		delete m_vehicle;
	if(m_raycaster)
		delete m_raycaster;
}

void Robot::tick(gkScalar rate)
{
	updateVehicle(rate);
}

void Robot::presubtick(gkScalar rate)
{

}

void Robot::subtick(gkScalar rate)
{
	for(UTsize i=0; i<m_wheels.size(); i++)
	{
		gkWheelProperties* wheel = m_wheels[i];
		btWheelInfo& btwheel = m_vehicle->getWheelInfo(i);

		// Rotate the wheel when not in contact
		if(!btwheel.m_raycastInfo.m_isInContact)
		{
		    //RIGHT
			if(m_gazR > 0.05f && i==0 )
				btwheel.m_deltaRotation = m_gazR;
			else
				btwheel.m_deltaRotation *= 0.995;

            //LEFT
            if(m_gazL > 0.05f && i==1 )
				btwheel.m_deltaRotation = m_gazL;
			else
				btwheel.m_deltaRotation *= 0.995;
		}

		btwheel.m_rotation += btwheel.m_deltaRotation;
	}
}

void Robot::createVehicle()
{
	if(m_object)
	{
		if (!m_object->isInstanced())
			m_object->createInstance();

		m_dynamicWorld = m_scene->getDynamicsWorld();
		m_dynamicWorld->addListener(this);

		m_chassis = ((gkRigidBody*)m_object->getPhysicsController())->getBody();
		m_chassis->setActivationState(DISABLE_DEACTIVATION);

		m_raycaster = new btDefaultVehicleRaycaster(m_dynamicWorld->getBulletWorld());

		m_vehicle = new btRaycastVehicle(m_tuning, m_chassis, m_raycaster);
		m_vehicle->setCoordinateSystem(0, 2, 1);

		m_dynamicWorld->getBulletWorld()->addVehicle(m_vehicle);

		for(UTsize i=0; i<m_wheels.size(); i++)
		{
			gkWheelProperties* wheel = m_wheels[i];
			btVector3 cp(wheel->m_connectionPoint.x, wheel->m_connectionPoint.y, wheel->m_connectionPoint.z);
			btVector3 wd(wheel->m_wheelDirection.x, wheel->m_wheelDirection.y, wheel->m_wheelDirection.z);
			btVector3 wa(wheel->m_wheelAxle.x, wheel->m_wheelAxle.y, wheel->m_wheelAxle.z);

			btWheelInfo& btwheel = m_vehicle->addWheel(cp , wd, wa, wheel->m_restLength, wheel->m_radius, m_tuning, wheel->m_isFront);

			btwheel.m_suspensionStiffness = wheel->m_stiffness;
			btwheel.m_wheelsDampingRelaxation = wheel->m_dampingRelax;
			btwheel.m_wheelsDampingCompression = wheel->m_dampingComp;
			btwheel.m_frictionSlip = wheel->m_friction;
			btwheel.m_rollInfluence = wheel->m_rollInfluence;
			btwheel.m_maxSuspensionTravelCm = wheel->m_travelDistCm;
		}
	}
}

void Robot::updateTransmition(gkScalar rate)
{
		float wheelRpm;
		if(m_vehicle->getNumWheels() >0)
			wheelRpm = 60 * getCurrentSpeedKmHour() / (3.6f * 2 * gkPi * m_vehicle->getWheelInfo(0).m_wheelsRadius);
		else
			wheelRpm = 0;
}

void Robot::updateVehicle(gkScalar rate)
{
	float wheelTorqueL,wheelTorqueR;

	updateTransmition(rate);

	//gearRatio = (m_gearBox)? m_gearBox->getCurrentRatio():1;
	wheelTorqueL = m_gazL * m_engineTorque / 2.0;
	wheelTorqueR = m_gazR * m_engineTorque / 2.0;

	btTransform trans;
	gkTransformState gtrans;

	int i;
	for (i = 0; i < m_vehicle->getNumWheels(); i++)
	{
		btWheelInfo& btwheel = m_vehicle->getWheelInfo(i);

		if(i==0)
			m_vehicle->applyEngineForce(wheelTorqueR*2.0, i);
        if(i==1)
			m_vehicle->applyEngineForce(wheelTorqueL*2.0, i);


		//synchronize the wheels with the (interpolated) chassis worldtransform
		m_vehicle->updateWheelTransform(i, true);

		//update wheels position
		trans = btwheel.m_worldTransform;
		gtrans.setIdentity();
		gtrans.loc = gkVector3(trans.getOrigin());
		gtrans.rot = gkQuaternion(trans.getRotation().w(), trans.getRotation().x(), trans.getRotation().y(), trans.getRotation().z());
		m_wheels[i]->m_object->setTransform(gtrans);

		//debugdraw
//        btCollisionShape *wheelShape = new btCylinderShapeX(btVector3(0.25, btwheel.m_wheelsRadius, btwheel.m_wheelsRadius));
//        m_dynamicWorld->debugDrawObject(trans, wheelShape, btVector3(0,1,0));
//        delete wheelShape;
	}
}

void Robot::addWheel(gkGameObject* object, gkScalar radius, gkVector3 connectionPoint, gkVector3 wheelDirection,
                         gkVector3 wheelAxle, bool isFront, gkScalar restLength, gkScalar stiffness, gkScalar dampingRelax,
                         gkScalar dampingComp, gkScalar friction, gkScalar roll, gkScalar travelDist)
{
	gkWheelProperties* wheel = new gkWheelProperties();

	wheel->m_object = object;
	wheel->m_radius = radius;
	wheel->m_isFront = isFront;
	wheel->m_connectionPoint = connectionPoint;
	wheel->m_wheelDirection = wheelDirection;
	wheel->m_wheelAxle = wheelAxle;
	wheel->m_restLength = restLength;
	wheel->m_stiffness = stiffness;
	wheel->m_dampingRelax = dampingRelax;
	wheel->m_dampingComp = dampingComp;
	wheel->m_friction = friction;
	wheel->m_rollInfluence = roll;
	wheel->m_travelDistCm = travelDist;

	m_wheels.push_back(wheel);
}

gkScalar Robot::getVelocityEulerZ(void)
{
	gkQuaternion rot;
	gkVector3 eul;

	if(!m_vehicle)
		return 0;

	gkVector3 dir = gkVector3(m_vehicle->getRigidBody()->getLinearVelocity());

	if ( gkAbs(dir.x) < 0.7 && gkAbs(dir.y) < 0.7)
	{
		rot = m_object->getWorldOrientation();
		eul = gkMathUtils::getEulerFromQuat(rot, true);

		return eul.z;
	}

	dir.z = 0;
	rot = dir.getRotationTo(gkVector3::UNIT_Y);
	eul = gkMathUtils::getEulerFromQuat(rot, true);

	return -eul.z;
}
