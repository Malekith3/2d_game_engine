SRC_FILES = $(shell find . -name '*.cpp')
CC = g++
LANG_STD = -std=c++17
COMPILER_FLAGS = -Wall -Wfatal-errors
INCLUDE_PATH = -I"./libs/" -I/opt/homebrew/include -L/opt/homebrew/lib
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua5.4
GAME_OBJECT = game_engine

build:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATH) $(SRC_FILES) $(LINKER_FLAGS) -o $(GAME_OBJECT);

all: build run clean

run:
	./$(GAME_OBJECT)

clean:
	rm -f $(GAME_OBJECT)



