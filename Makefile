CC=cc
CFLAGS=-Wextra -Wall -pedantic -std=c11

# Link Windows libraries.
ifeq ($(OS), Windows_NT)
	LDFLAGS=-lws2_32

# Link UNIX libraries.
else
	LDFLAGS=
endif

all:
	$(CC) $(CFLAGS) src/*.c -o abra $(LDFLAGS)

debug : CFLAGS += -DDEBUG -g
debug : all

.PHONY : clean
clean :
	rm -rf abra *.dSYM
