# Define variables
CC = g++
CFLAGS = -Wall -g
SRC_DIR = src
BUILD_DIR = build
TARGET = ipc_test
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJ_FILES)
$(CC) $(CFLAGS) -o $@ $^

# Rule to build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
mkdir -p $(BUILD_DIR)
$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove build artifacts
clean:
rm -rf $(BUILD_DIR) $(TARGET)
