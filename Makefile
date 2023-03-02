CC      := gcc
CFLAGS  := -std=gnu11 -Wall -Wextra -g
TARGET  := maze_explorer

SRCDIR  := src
INCDIR  := include

SRC     := $(wildcard $(SRCDIR)/*.c)
OBJ     := $(SRC:%.c=%.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -I$(INCDIR) -o $@ $^

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -f $(SRCDIR)/*.o $(TARGET)
