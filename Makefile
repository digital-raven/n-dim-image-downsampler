CC=g++
BIN=downsample
SOURCES=main.cpp NDimImage.cpp
all:
	$(CC) --std=c++0x -o $(BIN) $(SOURCES) -pthread

clean:
	rm $(BIN)
