
# files
main = main.c

# binaries
ASHELL = ashell

# cflags
CC = gcc
STD = -std=c2x
WARN = -Wpedantic
DEBUG = -g -DDEBUG
SANITIZE = -fsanitize=undefined,address
CFLAGS = $(STD) $(WARN) $(DEBUG) $(SANITIZE)


all: $(ASHELL)

$(ASHELL): $(main) makefile
	$(CC) $(main) $(CFLAGS) -o $@



clean:
	rm -f $(ASHELL)
