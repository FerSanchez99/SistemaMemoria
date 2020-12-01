CFLAGS = -O
CC = g++
SRC = main.cpp SistemaMemoria.cpp MemoriaReal.cpp MemoriaSwap.cpp Proceso.cpp
OBJ = $(SRC:.cpp = .o)

SistemaDeMemoria: $(OBJ)
	$(CC) $(CFLAGS) -o SistemaDeMemoria $(OBJ)

clean:
	rm -f core *.o
