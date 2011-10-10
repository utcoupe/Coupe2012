#include "Input.h"

EventManager * EventManager::em = NULL;

bool * EventManager::getEvent()
{
    return isEvent;
}

bool EventManager::OnEvent(const SEvent& event)
{
    // check if user presses the key
    if (event.EventType==EET_KEY_INPUT_EVENT)
    {
        switch (event.KeyInput.Key)
        {
        case KEY_KEY_Z :
        {
            //Si il s'agit d'un appui sur Z
            if (event.KeyInput.PressedDown == true) isEvent[HAUT]=true;
            //Sinon c'est un relachement
            else isEvent[HAUT]=false;
            return true;
        }
        break;
        case KEY_KEY_S :
        {
            if (event.KeyInput.PressedDown == true) isEvent[BAS]=true;
            else isEvent[BAS]=false;
            return true;
        }
        break;
        case KEY_KEY_Q :
        {
            if (event.KeyInput.PressedDown == true) isEvent[KEYQ]=true;
            else isEvent[KEYQ]=false;
            return true;
        }
        break;
        case KEY_KEY_D :
        {
            if (event.KeyInput.PressedDown == true) isEvent[KEYD]=true;
            else isEvent[KEYD]=false;
            return true;
        }
        break;
        case KEY_KEY_A :
        {
            if (event.KeyInput.PressedDown == true) isEvent[KEYA]=true;
            else isEvent[KEYA]=false;
            return true;
        }
        break;
        case KEY_KEY_E :
        {
            if (event.KeyInput.PressedDown == true) isEvent[KEYE]=true;
            else isEvent[KEYE]=false;
            return true;
        }
        break;
        case KEY_F1 :
        {
            if (event.KeyInput.PressedDown == true) isEvent[CAM1]=true;
            else isEvent[CAM1]=false;
            return true;
        }
        break;
        case KEY_F2 :
        {
            if (event.KeyInput.PressedDown == true) isEvent[CAM2]=true;
            else isEvent[CAM2]=false;
            return true;
        }
        break;
        case KEY_KEY_R :
        {
            if (event.KeyInput.PressedDown == true) isEvent[C_HAUT]=true;
            else isEvent[C_HAUT]=false;
            return true;
        }
        break;
        case KEY_KEY_F :
        {
            if (event.KeyInput.PressedDown == true) isEvent[C_BAS]=true;
            else isEvent[C_BAS]=false;
            return true;
        }
        break;
        }
    }
    return false;
}
