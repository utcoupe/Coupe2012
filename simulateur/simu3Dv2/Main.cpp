#include "Window.h"
#include "Robot.h"
#include "Input.h"


class Main : public Tickable{
	Robot bot;
	Window * win;
	InputNode * inputNode;

    void setup(gkScene * scene){
        //ajout le robot à la fenetre
        bot.setup(scene);

        //ajoute les touche du clavier au robot
        inputNode = win->getTree()->createNode<InputNode>();
        inputNode->setVehicle(&bot);
    }

    //instruction boucle principale
    void tick(gkScalar rate){
        bot.tick(rate);
    }


public :

    void run(){
        win->run();
    }

    Main(){
        TestMemory;
        gkLogger::enable("Simulateur3D.log", true);

        //création de la fenetre
        win = new Window("media/map.blend","media/OgreKitStartup.cfg");
        win->addTickable(this);
    }

};

int main(int argc, char** argv)
{
    Main main;
	main.run();
	return 0;
}

