CXX = g++
CXXFLAGS = -Wall -std=c++11
OBJ = menu.o alphabet.o vigenere.o hill.o
TARGET = encrypt_program

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

menu.o: menu.cpp alphabet.h vigenere.h hill.h
alphabet.o: alphabet.cpp alphabet.h
vigenere.o: vigenere.cpp vigenere.h alphabet.h
hill.o: hill.cpp hill.h alphabet.h

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
