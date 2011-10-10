#ifndef INPUT_MANAGER
#define INPUT_MANAGER

#include "main.h"
#include IRRLICHTPATH

using namespace irr;
using namespace scene;
using namespace video;
using namespace std;
using namespace core;
using namespace io;

#define NB_KEY 14
enum {HAUT=0,KEYA,KEYE,KEYQ,KEYD,BAS,GAUCHE,DROITE,L_GAUCHE,L_DROITE,CAM1,CAM2,C_HAUT,C_BAS};

//a faire:
//charger les touche a partie d'un fichier config

class EventManager : public IEventReceiver
{
private:
    static EventManager * em;
    bool isEvent[NB_KEY-1];
    EventManager(){}
public:
    static EventManager * getEventManager(){
        if(em == NULL){
            em = new EventManager;
            for (int i=0;i<NB_KEY;i++)
                em->isEvent[i]=false;
        }
        return em;
    }
    bool OnEvent(const SEvent& event);
    bool * getEvent();
};

#endif
