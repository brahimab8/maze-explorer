CC       := gcc
CFLAGS   := -std=gnu11 -Wall -Wextra -g
TARGET   := maze_explorer

SRCDIR   := src
INCDIR   := include
BUILDDIR := build

SRC      := $(wildcard $(SRCDIR)/*.c)
OBJ      := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRC))

.PHONY: all clean

all: $(BUILDDIR) $(TARGET)

# Link step
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -I$(INCDIR) -o $@ $^

# Compile .c -> .o into build/
$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# Create build directory if missing
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR) $(TARGET)
