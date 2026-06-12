CXX = g++
CXXFLAGS = -std=c++11 -Wall -I.
TARGET = rabin.o

all: $(TARGET)

$(TARGET): rabin.cpp rabin.h alphabet.h
	$(CXX) $(CXXFLAGS) -c rabin.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)
