SRC_FILES = $(shell find . -name '*.cpp')

build:
	g++ -Wall -std=c++17 -fdiagnostics-color=always -I"./libs" $(SRC_FILES) -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua5.4 -o game_engine;

all: build run clean

run:
	./game_engine

clean:
	rm -f game_engine



