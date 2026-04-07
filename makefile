# source 
MAIN = main.c
DIR = dir.c
SRC = $(MAIN) $(DIR)
OBJ = $(SRC:.c=.o)


# target
TARGET = ashell

# cflags
LD = ld
CC = gcc
STD = -std=c2x
WARN = -Wpedantic
DEBUG = -g -DDEBUG
LIBS = -lutil
SANITIZE = -fsanitize=undefined,address
CFLAGS = $(STD) $(WARN) $(DEBUG) $(SANITIZE)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c  $< -o $@ 


clean:
	rm -f $(OBJ) $(TARGET)
