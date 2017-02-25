CC = g++
CFLAGS = `pkg-config --cflags cairomm-pdf-1.0 fontconfig`
CFLAGS += --std=c++11
LIBS = `pkg-config --libs cairomm-pdf-1.0 fontconfig`
BUILD_DIR := $(shell mkdir -p build)

re-chord: build/Block.o build/Config.o build/Font.o build/Fragment.o build/Leader.o build/Line.o build/Page.o build/Song.o build/main.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

build/Block.o: source/Block.cpp source/Block.h source/TextType.h
	$(CC) -c -o $@ $< $(CFLAGS)

build/Config.o: source/Config.cpp source/Config.h
	$(CC) -c -o $@ $< $(CFLAGS)

build/Font.o: source/Font.cpp source/Font.h
	$(CC) -c -o $@ $< $(CFLAGS)

build/Fragment.o: source/Fragment.cpp source/Fragment.h source/TextType.h
	$(CC) -c -o $@ $< $(CFLAGS)

build/Leader.o: source/Leader.cpp source/Leader.h
	$(CC) -c -o $@ $< $(CFLAGS)

build/Line.o: source/Line.cpp source/Line.h source/Block.h source/TextType.h
	$(CC) -c -o $@ $< $(CFLAGS)

build/Page.o: source/Page.cpp source/Page.h source/Block.h source/Config.h source/Fragment.h source/TextType.h
	$(CC) -c -o $@ $< $(CFLAGS)

build/Song.o: source/Song.cpp source/Song.h source/Block.h source/Line.h source/TextType.h
	$(CC) -c -o $@ $< $(CFLAGS)

build/main.o: source/main.cpp source/Block.h source/Config.h source/Font.h source/Fragment.h source/Leader.h source/Line.h source/Page.h source/Song.h source/TextType.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf build re-chord
