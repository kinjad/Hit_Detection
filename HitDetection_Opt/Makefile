CC=gcc
CFLAGS=-I.
DEPS = IsHit.h StreamBuffer.h  
OBJ = IsHit.o StreamBuffer.o main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

HitDetection: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)


