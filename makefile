CXX = g++
CXXFLAGS = -Wall -std=c++11
TARGET = encrypt_program

# Базовые исходники (всегда есть)
SOURCES = menu.cpp alphabet.cpp vigenere.cpp hill.cpp

# Проверка наличия дополнительных модулей
ifneq (,$(wildcard xor.cpp))
    SOURCES += xor.cpp
    CXXFLAGS += -DXOR_AVAILABLE
endif

ifneq (,$(wildcard transp.cpp))
    SOURCES += transp.cpp
    CXXFLAGS += -DTRANSP_AVAILABLE
endif

ifneq (,$(wildcard rabin.cpp))
    SOURCES += rabin.cpp
    CXXFLAGS += -DRABIN_AVAILABLE
endif

ifneq (,$(wildcard ecc.cpp))
    SOURCES += ecc.cpp
    CXXFLAGS += -DECC_AVAILABLE
endif

OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Зависимости
menu.o: menu.cpp alphabet.h vigenere.h hill.h
alphabet.o: alphabet.cpp alphabet.h
vigenere.o: vigenere.cpp vigenere.h alphabet.h
hill.o: hill.cpp hill.h alphabet.h

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
