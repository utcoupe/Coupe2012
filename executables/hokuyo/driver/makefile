hokuyoApp : src/mainApp.cpp comManager.o
	g++ -o hokuyoApp src/mainApp.cpp comManager.o -Wall -Werror `urg-config --cflags` `sdl-config --cflags`  `urg-config --libs` `sdl-config --libs` -lc -lpthread 

comManager.o : src/comManager.cpp
	g++ -o comManager.o -c src/comManager.cpp  -Wall -Werror `urg-config --cflags` `sdl-config --cflags`  `urg-config --libs` `sdl-config --libs` -lc -lpthread 

clean : 
	rm -f *.o
