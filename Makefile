CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
CPPFLAGS ?= -Iinclude
LDLIBS ?= -lm

BUILD := build
LIB := $(BUILD)/libcanids.a
DEMO := $(BUILD)/canids_demo
TEST := $(BUILD)/canids_tests

SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c,$(BUILD)/%.o,$(SRC))

.PHONY: all demo test clean

all: demo test

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJ)
	ar rcs $@ $^

$(DEMO): examples/canids_demo.c $(LIB)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIB) $(LDLIBS) -o $@

$(TEST): tests/test_canids.c $(LIB)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIB) $(LDLIBS) -o $@

demo: $(DEMO)
	./$(DEMO)

test: $(TEST)
	./$(TEST)

clean:
	rm -rf $(BUILD)
