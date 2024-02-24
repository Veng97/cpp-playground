# Compiler
CXX := clang++

# Common compiler flags
CXXFLAGS := -std=c++20 -Wall -Wextra

# Default build type is Debug
BUILD_TYPE ?= Release

# Set flags based on build type
ifeq ($(BUILD_TYPE),Release)
    CXXFLAGS += -O2
else ifeq ($(BUILD_TYPE),Debug)
    CXXFLAGS += -g -O0
else
    $(error Unsupported build type: $(BUILD_TYPE))
endif

# Source files directory
SRC_DIR := src

# Include files directory
INC_DIR := include

# Output directory
BUILD_DIR := build

# Executable name
TARGET := $(BUILD_DIR)/main

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Phony targets
.PHONY: all clean

# Default rebuild target
all: $(TARGET)

# Rule to link object files into executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Rule to compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# Create build directory if it does not exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean rule
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

run:
	$(TARGET)