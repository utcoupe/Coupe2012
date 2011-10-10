#include "Window.h"
#include "Map.h"
#include "Robot.h"

#include <iostream>


int main (int argc , char * argv[]) {

    Window * window = Window::getWindow();
    Map * map = Map::getMap();
    Robot bot;

    bot.load("media/bot.obj");
    map->load("media/map.obj");

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
