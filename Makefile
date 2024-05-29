CC := clang 

TARGET := main

SOURCE_DIR := ./src
BUILD_DIR := ./build

SOURCES := $(shell find ${SOURCE_DIR}/* -name '*.c')
OBJECTS := $(SOURCES:.c=.o)

INC_FLAGS := $(shell pkg-config --cflags glfw3)
INC_FLAGS += -I./libs

LIB_FLAGS := $(shell pkg-config --static --libs glfw3)
LIB_FLAGS += $(shell pkg-config --static --libs cglm)

CFLAGS := -Wall
CFLAGS += -O3

# Color Formatting
CC_GREEN := $(shell echo "\033[92;1m")
CC_BLUE := $(shell echo "\033[94;1m")
CC_YELLOW := $(shell echo "\033[93;1m")
CC_END := $(shell echo "\033[0m")

%.o: %.c
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

all: $(OBJECTS) 
	${CC} ${CFLAGS} ${INC_FLAGS} -o ${TARGET} ${SOURCES} ${LIB_FLAGS} 
	@echo "$(CC_GREEN)Build done!$(CC_END)"

exec: all
	./$(TARGET)

bun: exec

clean:
	rm $(TARGET) $(OBJECTS)
	@echo "$(CC_BLUE)Clean done!$(CC_END)"

