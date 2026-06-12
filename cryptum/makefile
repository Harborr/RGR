CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -fPIC
LDFLAGS = -ldl

SRC_DIR = src
CIPHER_DIR = ciphers
BUILD_DIR = build

TARGET = cryptum
SOURCES = $(shell find $(SRC_DIR) -name "*.cpp")
OBJECTS = $(SOURCES:%.cpp=$(BUILD_DIR)/%.o)

ALGORITHMS = vigenere hill xor_cipher transposition ecc rabin
LIBS = $(addprefix $(BUILD_DIR)/lib, $(addsuffix .so, $(ALGORITHMS)))

all: $(TARGET) $(LIBS)

$(TARGET): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/lib%.so: $(CIPHER_DIR)/%/*.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -shared $^ -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

install: all
	./scripts/install.sh