SHELL := bash

PROG_NAME := serial-terminal

INC_DIR := ./inc
SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin
INS_DIR := /usr/local/bin

CC := gcc
CFLAGS := -std=gnu11 -O3 -I$(INC_DIR)
LFLAGS :=

INST := install
IFLAGS := --owner=root --group=root --mode=775

SRC_FIL := $(wildcard $(SRC_DIR)/*.c)
OBJ_FIL := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FIL))
BIN_FIL := $(BIN_DIR)/$(PROG_NAME)
INS_FIL := $(INS_DIR)/$(PROG_NAME)

.PHONY: all
all: $(BIN_FIL)

.PHONY: install
install: $(INS_FIL)

.PHONY: clean
clean:
	rm -fr $(OBJ_DIR)
	rm -fr $(BIN_DIR)

.PHONY: uninstall
uninstall:
	rm -f $(INS_FIL)

$(INS_FIL): $(BIN_FIL)
	$(INST) $(IFLAGS) $< $@

$(BIN_FIL): $(OBJ_FIL)
	@if [ ! -d $(BIN_DIR) ] ; then \
		echo "mkdir -p $(BIN_DIR)" ; \
		mkdir -p $(BIN_DIR) ; \
	fi
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@if [ ! -d $(OBJ_DIR) ] ; then \
		echo "mkdir -p $(OBJ_DIR)" ; \
		mkdir -p $(OBJ_DIR) ; \
	fi
	$(CC) $(CFLAGS) -c $< -o $@
