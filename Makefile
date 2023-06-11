SHELL := bash

PROG_NAME := serial-terminal

SRC_DIR := ./src
LIB_DIR := ./lib
BIN_DIR := ./bin
INS_DIR := /usr/local/bin

CC := gcc
CFLAGS := -std=gnu11 -O3
LFLAGS :=

INST := install
IFLAGS := --owner=root --group=root --mode=775

SRC_FIL := $(wildcard $(SRC_DIR)/*.c)
LIB_FIL := $(patsubst $(SRC_DIR)/%.c,$(LIB_DIR)/%.o,$(SRC_FIL))
BIN_FIL := $(BIN_DIR)/$(PROG_NAME)
INS_FIL := $(INS_DIR)/$(PROG_NAME)

.PHONY: all
all: $(BIN_FIL)

.PHONY: install
install: $(INS_FIL)

.PHONY: clean
clean:
	rm -fr $(LIB_DIR)
	rm -fr $(BIN_DIR)

.PHONY: uninstall
uninstall:
	rm -f $(INS_FIL)

$(INS_FIL): $(BIN_FIL)
	$(INST) $(IFLAGS) $< $@

$(BIN_FIL): $(LIB_FIL)
	@if [ ! -d $(BIN_DIR) ] ; then \
		echo "mkdir -p $(BIN_DIR)" ; \
		mkdir -p $(BIN_DIR) ; \
	fi
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

$(LIB_DIR)/%.o: $(SRC_DIR)/%.c
	@if [ ! -d $(LIB_DIR) ] ; then \
		echo "mkdir -p $(LIB_DIR)" ; \
		mkdir -p $(LIB_DIR) ; \
	fi
	$(CC) $(CFLAGS) -c $< -o $@
