CXX = g++
CXXFLAGS = -std=c++11 -Wall
OBJ = main_ecc.o ecc.o alphabet.o
TARGET = ecc_tool

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

main_ecc.o: main_ecc.cpp ecc.h alphabet.h
	$(CXX) $(CXXFLAGS) -c main_ecc.cpp -o main_ecc.o

ecc.o: ecc.cpp ecc.h alphabet.h
	$(CXX) $(CXXFLAGS) -c ecc.cpp -o ecc.o

alphabet.o: alphabet.cpp alphabet.h
	$(CXX) $(CXXFLAGS) -c alphabet.cpp -o alphabet.o

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
