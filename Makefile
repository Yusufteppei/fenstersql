CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -Wno-unused-result -O2 -MMD -MP
OBJ = main.o data.o page.o pager.o parser.o 
TARGET = main


DEPS = $(OBJ:.o=.d)


# The first rule is the one 'make' runs by default
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -f *.o $(TARGET)
 
test: main.o data.o page.o pager.o tests/test_suite.c
	$(CC) $(CFLAGS) $^ -o run_tests
	./run_tests
	rm run_tests
