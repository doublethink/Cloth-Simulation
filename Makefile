CC = g++
LPATH = -L/usr/pkg/lib
LDPATH = -Wl,-R/usr/pkg/lib 
CFLAGS=-g -Wall
LIBS=-lGL -lglut -lGLU -ljpeg -lpng12 -lm -lGLEW
IPATH= -I/usr/pkg/include

all: Project
Project : main.o G308_ImageLoader.o Node.o Cloth.o Spotlight.o textfile.o Shader.o Triangle.o Connection.o
	$(CC) -o Project main.o G308_ImageLoader.o Node.o Cloth.o Spotlight.o textfile.o Shader.o  Triangle.o Connection.o $(LIBS) $(LPATH) $(LDPATH)
.cpp.o: 
	$(CC) $(CFLAGS) -c -o $@ $^ $(IPATH)
clean :
	rm -rf *.o
	rm Project