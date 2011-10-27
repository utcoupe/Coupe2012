#include "Window.h"
#include "Robot.h"
#include "Input.h"

#include "mycircbot.h"


class Main : public Tickable{
	Robot bot;
	Window * win;
	InputNode * inputNode;

    void setup(gkScene * scene){
        //ajout le robot à la fenetre
        bot.setup(scene,300,300,-90.0);

        //ajoute les touche du clavier au robot
        inputNode = win->getTree()->createNode<InputNode>();
        inputNode->setVehicle(&bot);
        try
        {
            //Create new instance of our custom bot
            MyCIrcBot *p = new MyCIrcBot;

            //We want verbose messages turned on.
            p->setVerbose(true);

            //Connect to our irc server
            p->connect("localhost");

            //Join our channel
            p->joinChannel("#test");
            p->joinChannel("#test2");
        }
        catch(Exception &e)
        {
            //Error, so let's print it and exit 1
            cout << e.what() << endl;
            return;
        }
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

