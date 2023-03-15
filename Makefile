# Top‐level Makefile

# Compiler settings
CC       := gcc
CFLAGS   := -std=gnu11 -Wall -Wextra -g
INCDIR   := include

# Source layout
SRCDIR    := src
BUILDDIR  := build
TARGET    := maze_explorer

# All library sources = everything under src except main.c
LIB_SRCS  := $(filter-out $(SRCDIR)/main.c,$(wildcard $(SRCDIR)/*.c))
LIB_OBJS  := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(LIB_SRCS))
MAIN_OBJ  := $(BUILDDIR)/main.o

.PHONY: all test clean coverage

all: $(BUILDDIR) $(TARGET)

$(BUILDDIR):
	mkdir -p $@

# compile library .c → .o
$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# compile main.c
$(MAIN_OBJ): $(SRCDIR)/main.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# link final executable
$(TARGET): $(LIB_OBJS) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -I$(INCDIR) -o $@ $^

# Coverage 
.PHONY: coverage
coverage:
	@./scripts/coverage.sh

# Clean only the core build
.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(TARGET) coverage-report coverage.info

# Build library first, then run tests under test/
.PHONY: test
test: all
	@echo "Library built; now running CMocka tests…"
	$(MAKE) -C test test
