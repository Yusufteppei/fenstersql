CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -Wno-unused-result -Wno-unused-parameter -Wno-unused-variable -O2 -MMD -MP

# 1. Alle Quellen in src/ finden (inklusive btree.c!)
SRCS = $(wildcard src/*.c)

# 2. Objekte sauber nach obj/ delegieren
# Wir nehmen den Dateinamen ohne Pfad und setzen obj/ davor
OBJS = $(SRCS:src/%.c=obj/%.o)

TARGET = build/main
DEPS = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p build
	$(CC) $(OBJS) -o $(TARGET)

# 3. Die Regel, die Ordnung schafft
obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

# 4. Der "Deep Clean"
clean:
	rm -rf obj build
	rm -f src/*.d src/*.o  # Löscht die alten Überreste in src/
	rm -f *.o *.d          # Löscht Überreste im Hauptverzeichnis

test: $(filter-out obj/main.o, $(OBJS)) tests/test_suite.c
	$(CC) $(CFLAGS) $^ -o run_tests
	./run_tests
	rm run_tests
