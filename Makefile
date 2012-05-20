PROGRAM = deltasim

SOURCES = $(PROGRAM).c
CC = $(ARCH)gcc

.PHONY: all program clean

all:
	$(CC) $(SOURCES) -o $(PROGRAM).exe -lglfw -lopengl32 -lglu32 -std=c99
	size $(PROGRAM).exe

debug:
	$(CC) $(SOURCES) -o $(PROGRAM)-debug.txt -E -C -lglfw -lopengl32 -lglu32 -std=c99


clean:
	rm -rf $(PROGRAM).exe