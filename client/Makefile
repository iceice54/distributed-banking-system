SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
# Compiler and Flags
CXX      = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -g
# -lws2_32 links the Winsock library
# -mconsole ensures the linker looks for main() instead of WinMain()
LDFLAGS  = -lws2_32

# Source and Object files
SRCS     := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
TARGET   := $(BIN_DIR)/BankClient.exe

# Default rule
all: $(TARGET)

$(BIN_DIR) $(OBJ_DIR):
	mkdir $@

# Link the executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rd /s /q $(OBJ_DIR) $(BIN_DIR)

# Run the client
run: all
	./$(TARGET)
