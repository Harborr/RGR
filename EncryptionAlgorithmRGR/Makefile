.PHONY: all clean bx bt ba

all: bx bt ba

bx:
	cd Plugins/Xor && g++ -std=c++17 -fPIC -shared -o libxorciph.so Xor.cpp

bt:
	cd Plugins/Transp && g++ -std=c++17 -fPIC -shared -o libtransp.so Transp.cpp

ba:
	g++ -std=c++17 -o bin/crypt main.cpp UI/Menu.cpp FileHandler/FileHandler.cpp KeyGen/KeyGen.cpp -ldl -lstdc++fs -I.

clean:
	rm -f bin/crypt
	rm -f Plugins/Xor/*.so Plugins/Transp/*.so
