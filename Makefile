all:
	# g++ -I ./include/SDL2 -L ./lib -o main ./src/main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# Compiler and flags
CXX = g++
CXXFLAGS = -I ./include/SDL2 -L ./lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# Source files and output
SRC = ./src/main.cpp
OUT = main

# Default build rule
all:
	$(CXX) $(SRC) -o $(OUT) $(CXXFLAGS)

# Clean build
clean:
	del $(OUT).exe
