#include "Window.h"
#include "Map.h"
#include "Robot.h"
#include "Irc.h"

#include <iostream>


int main (int argc , char * argv[]) {

    Window * window = Window::getWindow();
    Map * map = Map::getMap();
    map->load("media/map.dae");

    Robot bot;
    bot.load("media/bot.obj");
    cout<<"IRC status : "<<bot.connect(new sf::IPAddress ("127.0.0.1"),6667,"asserv","asserv")<<endl;
    bot.channelJoin("#test");


    bool run=true;

    cout<<"main"<<endl;
    while (run) {
        run=window->beginWindowsRender();
        map->run();
        bot.run();
        window->endWindowsRender();
    }

    return EXIT_SUCCESS;
}
