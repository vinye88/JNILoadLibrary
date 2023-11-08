CC = g++
CFLAGS = -shared -fPIC
INCLUDES = -I"${JAVA_HOME}/include" -I"${JAVA_HOME}/include/win32"

SRC_DIR = main/cpp
OBJ_DIR = main/obj
BIN_DIR = main/resources/win32-x86-64

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
TARGET = $(BIN_DIR)/nativeExample.dll

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
