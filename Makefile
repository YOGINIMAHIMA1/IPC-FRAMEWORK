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

