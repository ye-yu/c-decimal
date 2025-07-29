# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
LDFLAGS =

# Directories
SRC_DIR = src
BUILD_DIR = build
TARGET = my_program

# Find all .c files in the source directory
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Replace the .c extension with .o and put them in the build directory
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# The first rule is the default goal
all: $(TARGET)

# Rule to link the final executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Rule to compile a source file into an object file in the build directory
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR) # Create build directory if it doesn't exist
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up build artifacts
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean