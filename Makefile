PROGRAM = sliced

SOURCES = $(PROGRAM).c
CC = $(ARCH)gcc

.PHONY: all program clean

all:
	$(CC) $(SOURCES) -o $(PROGRAM).exe -lglfw -lopengl32 -lglu32 -std=c99
	size $(PROGRAM).exe

clean:
	rm -rf $(PROGRAM).exe