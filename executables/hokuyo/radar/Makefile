C++_FLAG = -Wall -Werror `urg-config --cflags` `sdl-config --cflags`  `urg-config --libs` `sdl-config --libs` -lc -lpthread 


do : radarDriver.cpp	
	g++ -o raddriver.exe radarDriver.cpp $(C++_FLAG)
