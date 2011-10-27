/*Fonction de liaison entre les touche et les commande du robot*/

#ifndef GKVEHICLENODE_H
#define GKVEHICLENODE_H

#include "OgreKit.h"
#include "Robot.h"

class InputNode : public gkLogicNode
{
public:

	gkKeyNode* m_upKeyNode;
	gkKeyNode* m_downKeyNode;
	gkKeyNode* m_leftKeyNode;
	gkKeyNode* m_rightKeyNode;
	gkKeyNode* m_spaceKeyNode;
	gkKeyNode* m_dKeyNode;
	gkKeyNode* m_cKeyNode;
	gkKeyNode* m_rKeyNode;

    gkLogicTree* m_tree;

	enum
	{
		UPDATE,
		FRONT,
		REAR,
		LEFT,
		RIGHT,
		ZROT,
		KMH,
		GEAR,
		RPM,
	};

	DECLARE_SOCKET_TYPE(UPDATE, bool);
	DECLARE_SOCKET_TYPE(FRONT, bool);
	DECLARE_SOCKET_TYPE(REAR, bool);
	DECLARE_SOCKET_TYPE(LEFT, bool);
	DECLARE_SOCKET_TYPE(RIGHT, bool);
	DECLARE_SOCKET_TYPE(ZROT, gkScalar);
	DECLARE_SOCKET_TYPE(KMH, int);
	DECLARE_SOCKET_TYPE(GEAR, int);
	DECLARE_SOCKET_TYPE(RPM, int);

	InputNode(gkLogicTree* parent, size_t id);
	virtual ~InputNode() {}

	bool evaluate(gkScalar tick);

	void setVehicle(Robot* v)   {
	    m_vehicle = v;
	    m_upKeyNode = m_tree->createNode<gkKeyNode>();
        m_upKeyNode->setKey(KC_AKEY);

        m_downKeyNode = m_tree->createNode<gkKeyNode>();
        m_downKeyNode->setKey(KC_EKEY);

        m_leftKeyNode = m_tree->createNode<gkKeyNode>();
        m_leftKeyNode->setKey(KC_DKEY);

        m_rightKeyNode = m_tree->createNode<gkKeyNode>();
        m_rightKeyNode->setKey(KC_QKEY);

        getFRONT()->link(m_upKeyNode->getIS_DOWN());
        getREAR()->link(m_downKeyNode->getIS_DOWN());
        getLEFT()->link(m_leftKeyNode->getIS_DOWN());
        getRIGHT()->link(m_rightKeyNode->getIS_DOWN());
    }

private:
	Robot* m_vehicle;
	gkScalar   m_steer;
};

#endif // GKVEHICLENODE_H
