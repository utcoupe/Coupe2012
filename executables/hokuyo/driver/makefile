hokuyoApp : src/mainApp.cpp comManager.o argManager.o urgDriver.o urgDriverHelp.o urgException.o urgDriverCompute.o 
	g++ -o hokuyoApp src/mainApp.cpp urgException.o comManager.o urgDriverCompute.o  argManager.o urgDriver.o urgDriverHelp.o -Wall -Werror `urg-config --cflags` `sdl-config --cflags`  `urg-config --libs` `sdl-config --libs` -lc -lpthread 

urgException.o : src/urgException.cpp
	g++ -o urgException.o -c src/urgException.cpp  -Wall -Werror `urg-config --cflags` `sdl-config --cflags`  `urg-config --libs` `sdl-config --libs` -lc -lpthread 

urgDriverCompute.o : src/urgDriverCompute.cpp
	g++ -o urgDriverCompute.o -c src/urgDriverCompute.cpp  -Wall -Werror `urg-config --cflags` `sdl-config --cflags`  `urg-config --libs` `sdl-config --libs` -lc -lpthread 

urgDriverHelp.o : src/urgDriverHelp.cpp
	g++ -o urgDriverHelp.o -c src/urgDriverHelp.cpp  -Wall -Werror `urg-config --cflags` `sdl-config --cflags`  `urg-config --libs` `sdl-config --libs` -lc -lpthread 

urgDriver.o : src/urgDriver.cpp
	g++ -o urgDriver.o -c src/urgDriver.cpp  -Wall -Werror `urg-config --cflags` `sdl-config --cflags`  `urg-config --libs` `sdl-config --libs` -lc -lpthread 

comManager.o : src/comManager.cpp
	g++ -o comManager.o -c src/comManager.cpp  -Wall -Werror `urg-config --cflags` `sdl-config --cflags`  `urg-config --libs` `sdl-config --libs` -lc -lpthread 

argManager.o : src/argManager.cpp
	g++ -o argManager.o -c src/argManager.cpp

clean : 
	rm -f *.o
