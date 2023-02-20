CC=g++
CFLAGS=-I.
LDFLAGS = -L/usr/include/absl/container
INCLUDE = -Iinclude -I/usr/local/include -I/usr/include

interpreter: interpreter.cpp
	$(CC) $(LDFLAGS) -std=c++17 -Wall -O2 -o interpreter interpreter.cpp

interpretersimplest: interpretersimplest.cpp
	$(CC) -std=c++17 -Wall -O2 -o interpretersimplest interpretersimplest.cpp