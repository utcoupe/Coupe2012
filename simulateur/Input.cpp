#include "Input.h"

InputNode::InputNode(gkLogicTree* parent, size_t id)
	:    gkLogicNode(parent, id), m_vehicle(0), m_steer(0),m_upKeyNode(0), m_downKeyNode(0),
	  m_leftKeyNode(0), m_rightKeyNode(0), m_spaceKeyNode(0), m_rKeyNode(0), m_dKeyNode(0), m_cKeyNode(0)
{
    m_tree=parent;
	ADD_ISOCK(UPDATE, true);
	ADD_ISOCK(FRONT, false);
	ADD_ISOCK(REAR, false);
	ADD_ISOCK(LEFT, false);
	ADD_ISOCK(RIGHT, false)
	ADD_OSOCK(ZROT, 0);
	ADD_OSOCK(KMH, 0);
	ADD_OSOCK(GEAR, 0);
	ADD_OSOCK(RPM, 0);
}

bool InputNode::evaluate(gkScalar tick)
{
	bool front = GET_SOCKET_VALUE(FRONT);
	bool rear = GET_SOCKET_VALUE(REAR);
	bool left = GET_SOCKET_VALUE(LEFT);
	bool right = GET_SOCKET_VALUE(RIGHT);

	if (m_vehicle)
	{

		m_vehicle->setGazR(0.0);m_vehicle->setGazL(0.0);

		if (front)
		{
				m_vehicle->setGazR(1.0);
		}else if(right){
                m_vehicle->setGazR(-1.0);
		}

		if (rear)
		{
				m_vehicle->setGazL(1.0);
		}else if(left){
                m_vehicle->setGazL(-1.0);
		}
		SET_SOCKET_VALUE(ZROT, m_vehicle->getVelocityEulerZ());
		SET_SOCKET_VALUE(KMH, (int)(m_vehicle->getCurrentSpeedKmHour() + 0.5));
		SET_SOCKET_VALUE(RPM, m_vehicle->getCurrentRpm());
	}

	bool update = GET_SOCKET_VALUE(UPDATE);
	return update && m_vehicle;
}
