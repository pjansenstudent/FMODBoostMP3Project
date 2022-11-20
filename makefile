all: build

build: main.cpp
	g++ main.cpp -I/fmod_files/libfmodex64.so -lfmodex64 -lboost_system -lboost_filesystem -o player

run: player
	./player

clean:
	rm player
