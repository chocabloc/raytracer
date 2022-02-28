# Source and Object files
SRC = $(shell find . -name "*.c")
OBJ = $(SRC:.c=.o)

# Compiler options
CC = gcc
LD = gcc
CFLAGS = -std=gnu2x -Ofast -flto -march=native -Wall -Wno-missing-braces
LDFLAGS = -flto -fopenmp -L/usr/X11R6/lib -lX11 -lm

# Output binary
OUT = ./rt

# Rules
.phony = clean run

all: $(OUT)

$(OUT): $(OBJ)
	$(LD) $(OBJ) $(LDFLAGS) -o $(OUT)
$(OBJ): %.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@
run: $(OUT)
	$(OUT)
clean:
	rm -f $(OBJ)
