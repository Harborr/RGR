CXX = g++
CXXFLAGS = -std=c++11 -Wall
OBJ = main_rabin.o rabin.o alphabet.o
TARGET = rabin_tool

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

main_rabin.o: main_rabin.cpp rabin.h alphabet.h
	$(CXX) $(CXXFLAGS) -c main_rabin.cpp -o main_rabin.o

rabin.o: rabin.cpp rabin.h alphabet.h
	$(CXX) $(CXXFLAGS) -c rabin.cpp -o rabin.o

alphabet.o: alphabet.cpp alphabet.h
	$(CXX) $(CXXFLAGS) -c alphabet.cpp -o alphabet.o

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
