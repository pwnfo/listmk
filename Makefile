CC = gcc
CFLAGS = -Wall -c
LDFLAGS = -g

C_SOURCE = $(wildcard src/*.c)
H_SOURCE = $(wildcard src/*.h)
OBJ = $(C_SOURCE:.c=.o)

all: listmk

listmk: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f src/*.o listmk
