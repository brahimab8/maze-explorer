# Compiler settings
CC       := gcc
CFLAGS   := -std=gnu11 -Wall -Wextra -g
LDFLAGS  :=

# When COVERAGE=1 is set, instrument everything
ifdef COVERAGE
  CFLAGS  += --coverage
  LDFLAGS += --coverage
endif

INCDIR   := include

# Source layout
SRCDIR   := src
BUILDDIR := build
TARGET   := maze_explorer

# All library sources except main.c
LIB_SRCS := $(shell find $(SRCDIR) -type f -name '*.c' -not -name 'main.c')
LIB_OBJS := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(LIB_SRCS))
MAIN_OBJ := $(BUILDDIR)/main.o

# Build subdirs
OBJDIRS  := $(sort $(dir $(LIB_OBJS) $(MAIN_OBJ)))


.PHONY: all test clean coverage dirs

# Default: build the executable
all: $(TARGET)

# Link final executable
$(TARGET): $(LIB_OBJS) $(MAIN_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -I$(INCDIR) -o $@ $^

# Compile library sources (recursively), preserving subdirs
$(BUILDDIR)/%.o: $(SRCDIR)/%.c | dirs
	$(CC) $(CFLAGS) $(LDFLAGS) -I$(INCDIR) -c $< -o $@

# Compile main.c
$(MAIN_OBJ): $(SRCDIR)/main.c | dirs
	$(CC) $(CFLAGS) $(LDFLAGS) -I$(INCDIR) -c $< -o $@

# Ensure build subdirs exist
dirs:
	mkdir -p $(BUILDDIR)
	$(foreach d,$(OBJDIRS), mkdir -p $(d);)

# test:     build & run the tests (no coverage)
test: all
	@echo "Library built; now running CMocka tests…"
	$(MAKE) -C test COVERAGE=0 test

# coverage: full instrumentation, run tests, then capture & report
coverage:
	@echo "=== Running coverage build ==="
	$(MAKE) clean
	$(MAKE) COVERAGE=1 all

	@echo "=== Running coverage‐instrumented tests ==="
	$(MAKE) -C test COVERAGE=1 test

	@echo "=== Capturing coverage data ==="
	lcov --capture --directory . --output-file coverage.info

	@echo "=== Filtering out system and tests ==="
	lcov --remove coverage.info '*/test/*' --output-file coverage.info

	@echo "=== Generating HTML report ==="
	genhtml coverage.info --output-directory coverage-report

	@echo
	@echo ">> Coverage report: coverage-report/index.html"

clean:
	rm -rf $(BUILDDIR) $(TARGET) coverage-report coverage.info
	@echo "Cleaning test artifacts…"
	$(MAKE) -C test clean
