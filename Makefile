ifneq (, $(shell which g++-8 &> /dev/null ))
CXX=g++-8
else
CXX=g++
endif
#CXX=clang
XLOGINXX=xyadlo00

OBJ=src/*
BIN=kry

CXXFLAGS:=-Wall -Wextra -Wsuggest-override -Wnull-dereference -Wshadow -Wold-style-cast -pedantic -std=c++2a

LINK.o = $(LINK.cpp)

all: CXXFLAGS += -Ofast -march=native -flto
all: kry

debug: CXXFLAGS += -g3 -fsanitize=address,undefined -fno-omit-frame-pointer
debug: kry

kry: $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o $(BIN) -lgmp

pack:
	zip $(XLOGINXX).zip *.cpp *.hpp  Makefile doc.pdf

dep:
	g++ *.cpp -MM >> Makefile