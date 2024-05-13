CC := clang 

CFLAGS += -Wall
# CFLAGS += -Werror
CFLAGS += -Wextra
CFLAGS += -pedantic

RAYLIB_PATH := ext/raylib-5.0

INC += -I$(RAYLIB_PATH)/src
LIBS += -L$(RAYLIB_PATH)/src
LIBS += -lraylib

SRC = $(wildcard *.c)
OBJ = $(SRC:%.c=$(BIN)/%.o)

BIN := bin
OUT := voronoi

RPATH = -rpath,$(RAYLIB_PATH)/src

SRC = $(wildcard *.c)

CUR_DIR := $(shell pwd)

.PHONY: all clean

$(OUT): $(OBJ)
	$(CC) $(LIBS) -Wl,$(RPATH) $^ -o bin/$@

all: build-raylib $(OUT) 

build-raylib:
	cd $(RAYLIB_PATH)/src && make -j4
	cd $(CUR_DIR)

$(BIN):
	mkdir -p $@

$(BIN)/%.o: %.c $(BIN)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(BIN)
