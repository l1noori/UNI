#workspace settings
INC_DIR = include
SRC_DIR = src
SOURCES = $(shell find $(SRC_DIR)/ -name '*.cc')
OBJECTS = $(SOURCES:.cc=.o)
DEPS = $(OBJECTS:.o=.d)
TARGET = main

# CXX compiler settings
CC = g++
CFLAGS = -march=native -I$(INC_DIR) -Wall -Wextra -Wpedantic

.PHONY: all exec clean debug release build
#build the program
build: release

#release build
release: CFLAGS += -O3 -DNDEBUG
release: all

#debugging build
debug: CFLAGS += -O0 -DDEBUG -ggdb3
debug: all

all: $(TARGET)
$(TARGET): $(OBJECTS)
		$(CC) $(CFLAGS) -o $@ $^

-include $(DEPS)
%.o: %.cc
		$(CC) $(CFLAGS) -MMD -o $@ -c $<

exec:
	./${TARGET}
run:
	./${TARGET} d file.txt
clean:
	rm -f ${OBJECTS} ${DEPS} ${TARGET}