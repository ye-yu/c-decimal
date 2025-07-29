# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude -Itests\unity\src
LDFLAGS =

# Directories
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build
TARGET = my_program

# Find all .c files in the source directory
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Replace the .c extension with .o and put them in the build directory
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Do the same for test files
TESTS = $(wildcard $(TEST_DIR)/*_test.c)
TEST_TARGET = test_runner

# The first rule is the default goal
all: $(TARGET)

# Rule to link the final executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Rule to compile a source file into an object file in the build directory
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR) # Create build directory if it doesn't exist
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile a source file into an object file in the build directory
$(BUILD_DIR)/%_test.o: $(TEST_DIR)/%_test.c
	@mkdir -p $(BUILD_DIR) # Create build directory if it doesn't exist
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up build artifacts
clean:
	rm -rf $(BUILD_DIR) $(TARGET)


$(BUILD_DIR)/unity.o:
	@mkdir -p $(BUILD_DIR) # Create build directory if it doesn't exist
	@echo "Compiling Unity test framework..."
	$(CC) $(CFLAGS) -c $(TEST_DIR)/unity/src/unity.c -o $(BUILD_DIR)/unity.o

%_test: $(BUILD_DIR)/%_test.o $(BUILD_DIR)/unity.o
	@echo "Running single test..."
	$(CC) $< $(BUILD_DIR)/unity.o -o $<.test.exe $(LDFLAGS)
	./$<.test.exe > $<.test.log 2> $<.test.err || \
	{ echo "Test failed. Check $<.test.log and $<.test.err for details."; exit 0; }
	@cat $<.test.log $<.test.err

%_test.exe: $(BUILD_DIR)/%_test.o $(BUILD_DIR)/unity.o
	@echo "Running single test..."
	$(CC) $< $(BUILD_DIR)/unity.o -o $(TEST_TARGET) $(LDFLAGS)

TEST_OBJS = $(patsubst $(TEST_DIR)/%_test.c, %_test, $(TESTS))
test: $(TEST_OBJS)


.PHONY: all clean