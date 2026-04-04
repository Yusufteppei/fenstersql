CC = gcc
# Added -Iinclude to find the generated parser header
CFLAGS = -Iinclude -Wall -Wextra -Wno-unused-result -Wno-unused-parameter -Wno-unused-variable -O2 -MMD -MP
# Link against flex library (-lfl)
LDFLAGS = -lfl #-lreadline

# 1. Define generated files
GEN_C = src/lexer.yy.c src/parser.tab.c
GEN_H = include/parser.tab.h

# 2. Update SRCS to include generated C files
SRCS = $(wildcard src/*.c)  src/lexer.yy.c src/parser.tab.c
# Filter out duplicates if wildcard already caught them
SRCS := $(sort $(SRCS))

OBJS = $(SRCS:src/%.c=obj/%.o)
TARGET = build/main
DEPS = $(OBJS:.o=.d)

all: $(TARGET)

# Rule to build the final executable
$(TARGET): $(OBJS)
	@mkdir -p build
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Rule for Bison: generates .c and .h
src/parser.tab.c $(GEN_H): src/parser.y
	bison -d src/parser.y -o src/parser.tab.c
	mv src/parser.tab.h include/

# Rule for Flex: generates .c
src/lexer.yy.c: src/lexer.l $(GEN_H)
	flex -o src/lexer.yy.c src/lexer.l

# Standard object rule
obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf obj build
	rm -f src/lexer.yy.c src/parser.tab.c include/parser.tab.h
	rm -f src/*.d src/*.o
	rm -f *.o *.d

.PHONY: all clean test
