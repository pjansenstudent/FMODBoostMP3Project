all: build

build: main.cpp
	g++ -I/fmod_files/libfmodex64.so -lfmodex64 -lboost_system -lboost_filesystem main.cpp -o player

run: player
	./player

clean:
	rm player
