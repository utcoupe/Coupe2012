# Compilateur
CC=g++

# Libs
# !!! il faut absolument mettre -lpthread !!!
LIBS=-lpthread 

# To compile all ^^
LIST=sample.exe
all : $(LIST)

#
sample.exe : sample.cpp ComManager.o
	$(CC) -o sample.exe sample.cpp ComManager.o $(LIBS)

ComManager.o : ComManager.cpp
	$(CC) -o ComManager.o -c ComManager.cpp $(LIBS)

# To delete tmp files
clean : 


