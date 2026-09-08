# Compilatore e flag di base
CC = gcc
CFLAGS = -Wall -Wextra -I./include

# Rilevamento automatico del sistema operativo
ifeq ($(OS),Windows_NT)
    # Impostazioni per Windows (MinGW)
    LDFLAGS = -lws2_32
    EXE = .exe
    RM = del /Q /F
else
    # Impostazioni per Linux/macOS
    # Includiamo già lpthread nel caso servisse in futuro
    LDFLAGS = -lpthread
    EXE =
    RM = rm -f
endif

# Il target "all" è quello di default quando digiti solo "make"
all: satellite parser

# Regola per compilare il satellite
satellite: src/satellite.c src/communication.c
	$(CC) $(CFLAGS) src/satellite.c src/communication.c -o satellite$(EXE) $(LDFLAGS)

# Regola per compilare il parser
parser: src/parser.c src/communication.c
	$(CC) $(CFLAGS) src/parser.c src/communication.c -o parser$(EXE) $(LDFLAGS)

# Regola per pulire lo spazio di lavoro ("make clean")
clean:
	$(RM) satellite$(EXE) parser$(EXE)