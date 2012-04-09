C++_FLAG = -Wall -Werror `urg-config --cflags` `sdl-config --cflags`  `urg-config --libs` `sdl-config --libs` -lc -lpthread 


do : radarDriver.cpp	
	g++ -o radDriver.exe radarDriver.cpp $(C++_FLAG)
