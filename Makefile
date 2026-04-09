CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -Wno-unused-result -Wno-unused-parameter -Wno-unused-variable -O2 -MMD -MP
LDFLAGS = -lfl -lreadline

# 1. Define generated files
GEN_C = src/lexer.yy.c src/parser.tab.c
GEN_H = include/parser.tab.h

# 2. Source and Objects
# Explicitly exclude the generated C files from the wildcard to prevent duplicates
# This catches everything in src/ AND your generated files
SRCS_BASE = $(wildcard src/*.c)
SRCS = $(sort $(SRCS_BASE) src/lexer.yy.c src/parser.tab.c)

# Map src/file.c to obj/file.o
OBJS = $(patsubst src/%.c, obj/%.o, $(SRCS))
TARGET = build/main
DEPS = $(OBJS:.o=.d)

all: $(TARGET)

# Rule to build the final executable
$(TARGET): $(OBJS)
	@mkdir -p build
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Rule for Bison: generates .c and .h
# We use $(GEN_H) as the target so other files can depend on it
$(GEN_C) $(GEN_H): src/parser.y
	@mkdir -p include
	bison -d src/parser.y -o src/parser.tab.c
	mv src/parser.tab.h include/

# Rule for Flex: depends on the Bison header
src/lexer.yy.c: src/lexer.l $(GEN_H)
	flex -o src/lexer.yy.c src/lexer.l

# Standard object rule
# CRITICAL: We add $(GEN_H) as a prerequisite so it is built first!
obj/%.o: src/%.c $(GEN_H)
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf obj build
	rm -f $(GEN_C) $(GEN_H)

.PHONY: all clean test
