CIBLE = chess
SRCS = Graphics.cpp Main.cpp ChessPieces.cpp Camera.cpp Mesh.cpp Game.cpp Texture.cpp
LIBS =  -lglut -lGLU -lGL -lm 

CC = g++
CPP = g++

FLAGS = -Wall -O2 -std=c++11
CFLAGS = $(FLAGS)
CXXFLAGS = $(FLAGS)

OBJS = $(SRCS:.cpp=.o)   

$(CIBLE): $(OBJS)
	g++ $(LDFLAGS) -o $(CIBLE) $(OBJS) $(LIBS) 
clean:
	rm -f  *~  $(CIBLE) $(OBJS)

