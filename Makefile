CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -O2
SRC_DIR = src
OBJ_DIR = build
TARGET = benchmark

SOURCES = $(SRC_DIR)/benchmark.c $(SRC_DIR)/sorts.c $(SRC_DIR)/generators.c $(SRC_DIR)/utils.c
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
