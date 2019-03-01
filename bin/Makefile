
########################################################
# CHANGE ME IF SUBDIRECTORIES WERE ADDED
########################################################

# Set compiler to gcc, give it gcc flags, linker flags, set name
CC = gcc
CFLAGS=
LFLAGS=-lm -fno-signed-zeros
EXE_NAME=a.exe
EXT=c

# Tells make what directories to look in for things
PROGRAM_SRC = ../src
P_HEAD = ../include
OBJ_DIR = objects

# Makes an 'array' of files in source and head
PROGRAMSOURCES = $(wildcard $(PROGRAM_SRC)/*.$(EXT))
HEADERS = $(wildcard $(P_HEAD)/*.h)

# Tells make what all the object files are called
OBJECTS=$(patsubst $(PROGRAM_SRC)/%.$(EXT), $(OBJ_DIR)/%.o, $(PROGRAMSOURCES))


# Links all of the objects together, recompiles if objects/headers changed
build:	$(OBJECTS) $(HEADERS)
	$(CC) $(LFLAGS) -o $(EXE_NAME) $(OBJECTS) -I$(P_HEAD) 

# For any %.o, compile it with this command
$(OBJ_DIR)/%.o:	$(PROGRAM_SRC)/%.$(EXT)
	$(CC) $(CFLAGS) -I'$(PROGRAM_SRC)' -I'$(P_HEAD)' -c $< -o $@ 

run:	build
	@echo ""	
	@./$(EXE_NAME)

clean:
	rm -rf $(OBJ_DIR)
	rm $(EXE_NAME)
	mkdir -p $(OBJ_DIR)
