CXX = g++
CXXFLAGS = -std=c++11 -Wall -I.
TARGET = ecc.o

all: $(TARGET)

$(TARGET): ecc.cpp ecc.h alphabet.h
	$(CXX) $(CXXFLAGS) -c ecc.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)
