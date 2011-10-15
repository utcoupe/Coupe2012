#ifndef INPUT_MANAGER
#define INPUT_MANAGER

#include "main.h"
#include IRRLICHTPATH

#include <map>

using namespace irr;
using namespace scene;
using namespace video;
using namespace std;
using namespace core;
using namespace io;
using namespace std;

class EventManager : public IEventReceiver
{
private:
    static EventManager * em;
    std::map<int,bool> keys;
    EventManager(){}
public:
    static EventManager * getEventManager(){
        if(em == NULL){
            em = new EventManager;
            //Window::getWindow()->getDevice()->setEventReceiver(em);
        }
        return em;
    }
    bool OnEvent(const SEvent& event);
    bool getEvent(int key){
        if(keys.count(key)>0)
            return keys[key];
        else
            return false;
    }
    void showMap(){
     std::map<int,bool>::iterator it;
          // show content:
          for ( it=keys.begin() ; it != keys.end(); it++ )
            cout << (*it).first << " => " << (*it).second << endl;
    }

};

#endif
