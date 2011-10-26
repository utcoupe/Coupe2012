#ifndef WINDOW_H
#define WINDOW_H

/* gére le GUI de l'application touches et affichage*/

#include <iostream>
#include "OgreKit.h"

class Tickable{
    public :
    virtual void tick(gkScalar rate) = 0;
    virtual void setup(gkScene* scene) = 0;
};

class Window : public gkCoreApplication, public gkWindowSystem::Listener
{
private:

	gkGameObject* m_camera;
	gkCameraNode* m_cameraNode;
	gkLogicTree* m_tree;
    Tickable * tickobj;
    gkString  m_blend;
	gkScene* pScene;
	gkBlendFile* pBlendFile;


    void createCamera()
    {
        gkObjectNode* centerObj = m_tree->createNode<gkObjectNode>();
        centerObj->setOtherObject("ChassisCollision");
    }

public :
    gkLogicTree* getTree(){return m_tree;}

    void addTickable(Tickable * t){
        tickobj = t;
    }

	Window(const gkString& blend, const gkString& startCfg): m_blend(gkUtils::getFile(blend))
	{
		gkPath path = gkUtils::getFile(startCfg);

		if (path.isFile())
		{
			m_prefs.load(path.getPath());
		}
		else
		{
			gkLogMessage("ERROR: FILE NOT FOUND ---> " << startCfg);
		}
	}

	void tick(gkScalar rate)
	{
		tickobj->tick(rate);
	}

	bool setup()
	{
		pBlendFile = gkBlendLoader::getSingleton().loadFile(m_blend, "", "MAP");

		if (pBlendFile)
		{
			pScene = pBlendFile->getMainScene();
			GK_ASSERT(pScene);
			pScene->createInstance();

			// add input hooks
			gkWindowSystem::getSingleton().addListener(this);

            m_camera=pScene->getMainCamera();
            m_tree = gkNodeManager::getSingleton().createLogicTree(pScene->getGroupName());
            tickobj->setup(pScene);
            createCamera();
            m_tree->solveOrder();
            m_camera->attachLogic(m_tree);

			return true;
		}
		else
		{
			gkLogMessage("ERROR: FILE NOT FOUND ---> " << m_blend);
			return false;
		}
	}

	void keyPressed(const gkKeyboard& key, const gkScanCode& sc)
	{
	}

	void keyReleased(const gkKeyboard& key, const gkScanCode& sc)
	{
		if (sc == KC_ESCKEY)
			m_engine->requestExit();
	}

public:
};

#endif // WINDOW_H
