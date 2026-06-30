CC     = gcc
CFLAGS = -Wall -Wextra -g -O0 -I include
BUILD  = build

ifeq ($(OS), Windows_NT)
    LIBS = -lSDL3 -lmingw32
else
    LIBS = -lSDL3
endif

SRCS   = src/main.c src/logic.c src/ui.c src/io.c
TARGET = $(BUILD)/game_of_life

.PHONY: all clean test_logic test_io test_ui test_all

all: $(TARGET)

$(BUILD):
	mkdir -p $(BUILD)

$(TARGET): $(SRCS) | $(BUILD)
	$(CC) $(CFLAGS) $(SRCS) $(LIBS) -o $(TARGET)

clean:
	rm -rf $(BUILD)

test_logic: tests/test_logic.c src/logic.c | $(BUILD)
	$(CC) $(CFLAGS) tests/test_logic.c src/logic.c -o $(BUILD)/test_logic
	./$(BUILD)/test_logic

test_io: tests/test_io.c src/io.c src/logic.c src/ui.c | $(BUILD)
	$(CC) $(CFLAGS) tests/test_io.c src/io.c src/logic.c src/ui.c $(LIBS) -o $(BUILD)/test_io
	./$(BUILD)/test_io

test_ui: tests/test_ui.c src/ui.c src/logic.c | $(BUILD)
	$(CC) $(CFLAGS) tests/test_ui.c src/ui.c src/logic.c $(LIBS) -o $(BUILD)/test_ui
	./$(BUILD)/test_ui

test_all: test_logic test_io test_ui
