CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -O2
SRC_DIR = src
OBJ_DIR = build

SOURCES = $(SRC_DIR)/benchmark.c $(SRC_DIR)/sorts.c $(SRC_DIR)/generators.c $(SRC_DIR)/utils.c
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

ifeq ($(OS),Windows_NT)
TARGET = benchmark.exe
LDFLAGS = -Wl,--stack,67108864
MKDIR_OBJ = if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
RM = if exist "$(OBJ_DIR)" rmdir /S /Q "$(OBJ_DIR)" & if exist "$(TARGET)" del /Q "$(TARGET)"
RUN = .\$(TARGET)
else
TARGET = benchmark
LDFLAGS =
MKDIR_OBJ = mkdir -p $(OBJ_DIR)
RM = rm -rf $(OBJ_DIR) $(TARGET)
RUN = ./$(TARGET)
endif

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS) -lm

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR_OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	$(RUN)

clean:
	$(RM)
