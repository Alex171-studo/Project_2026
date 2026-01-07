CC = gcc
CFLAGS = -Wall -Wextra -I./inc -g
LDFLAGS = -lm

SRC_DIR = src
INC_DIR = inc
BUILD_DIR = build
TARGET = eco_sensing

# Auto-detect sources
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Default Rule
all: $(BUILD_DIR) $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compile
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Create build dir
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean
clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET) save.bin log.txt
	rm -rf $(BUILD_DIR)

.PHONY: all clean
