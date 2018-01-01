CC=/home/kwan/emsdk/emscripten/incoming/em++

c3.js: c3.cpp c3.hpp $(wildcard c3-*.hpp)
	$(CC) -std=c++1z -Os -s WASM=1 -s ASSERTIONS=1 --bind -o c3.js c3.cpp
