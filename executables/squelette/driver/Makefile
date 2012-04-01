# Compilateur
CC=g++

# Libs
# !!! il faut absolument mettre -lpthread !!!
LIBS=-lpthread 

# To compile all ^^
LIST=sample.exe
all : $(LIST)

#
sample.exe : sample.cpp comManager.o
	$(CC) -o sample.exe sample.cpp comManager.o $(LIBS)

comManager.o : comManager.cpp
	$(CC) -o comManager.o -c comManager.cpp $(LIBS)

# To delete tmp files
clean : 


