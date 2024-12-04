CC := g++
CFLAGS := -std=c++17 -Wall -Wextra -g
INCLUDES := -I.
LIBS := -luser32
DEFINES :=

MKDIR := mkdir
RMDIR := rmdir /s /q

SRC_DIR := src
OBJ_DIR := obj
BUILD_DIR := bin

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
EXEC := sxgfx.exe

### ALL
all: $(BUILD_DIR)/$(EXEC)

clean: cleanexe

## EXECUTABLE
$(BUILD_DIR)/$(EXEC): $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $< $(DEFINES) $(INCLUDES)

$(BUILD_DIR) $(OBJ_DIR):
	$(MKDIR) $@

run:
	$(BUILD_DIR)/$(EXEC)

cleanexe:
	$(RMDIR) $(BUILD_DIR) $(OBJ_DIR)
