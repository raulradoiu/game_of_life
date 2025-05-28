CC = gcc
CFLAGS = -O2 -Wall
SRC = game_of_life.c
OUT = a.out

INPUT_FILES = $(wildcard InputData/data*.in)

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run: $(OUT)
	./$(OUT) $(INPUT_FILES)

clean:
	rm -f $(OUT)
