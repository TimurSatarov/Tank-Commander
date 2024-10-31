CC=gcc
CFLAGS=-Iinclude -lSDL2 -lSDL2_image -lm  
SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)
TARGET=tankCommander

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
