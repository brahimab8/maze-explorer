# test/Makefile

# =============================================================================
# Compiler settings & coverage switch
# =============================================================================
CC       := gcc
CFLAGS   := -std=gnu11 -Wall -Wextra -g -I../include
LDFLAGS  :=

# When COVERAGE=1 is set, instrument test code
ifdef COVERAGE
  CFLAGS  += --coverage
  LDFLAGS += --coverage
endif

CMOCKA   := -lcmocka
BINDIR   := bin

.PHONY: all test clean

# Build stubs.o
STUB_OBJ := $(BINDIR)/stubs.o
$(STUB_OBJ): stubs.c | $(BINDIR)
	$(CC) $(CFLAGS) -Itest -c $< -o $@

# Gather library objects (all .o under ../build, except main.o)
CORE_OBJS := $(filter-out ../build/main.o, \
              $(shell find ../build -type f -name '*.o'))

# Wrapping flags for stubbing out real game/menu calls
WRAP := \
  -Wl,--wrap=run_menu   \
  -Wl,--wrap=run_load_menu \
  -Wl,--wrap=load_game_slot \
  -Wl,--wrap=save_game_slot \
  -Wl,--wrap=run_settings_menu \
  -Wl,--wrap=input_init \
  -Wl,--wrap=input_get_line \
  -Wl,--wrap=free_maze \

# Tests exercising game_play also need draw_maze wrapped
GAME_PLAY := \
  game_init_cmocka \
  game_loop_cmocka \
  game_menu_cmocka \
  maze_cmocka \
  settings_cmocka \
  gameplay_play_cmocka 
  # timer_cmocka

$(addprefix $(BINDIR)/,$(GAME_PLAY)): WRAP += -Wl,--wrap=draw_maze

# game_loop test also wraps game_init, monsters_init, and items_init
$(BINDIR)/game_loop_cmocka: WRAP += \
    -Wl,--wrap=game_init \
    -Wl,--wrap=monsters_init \
    -Wl,--wrap=items_init

# player test also wraps can_step
$(BINDIR)/gameplay_player_cmocka: WRAP += -Wl,--wrap=can_step
# projectile test also wraps can_step
$(BINDIR)/gameplay_projectile_cmocka: WRAP += -Wl,--wrap=can_step

$(BINDIR)/save_load_cmocka: WRAP :=

# Discover test sources → binaries
TEST_SRCS := $(wildcard *_cmocka.c)
TEST_BINS := $(patsubst %.c,$(BINDIR)/%,$(TEST_SRCS))

all: $(STUB_OBJ) $(TEST_BINS)

test: all
	@echo "Running all CMocka tests…"
	@for t in $(TEST_BINS); do \
	  echo "==> $$t"; \
	  $$t || exit 1; \
	done
	@echo "All tests passed!"

# Pattern rule to build each test binary
$(BINDIR)/%: %.c $(STUB_OBJ) $(CORE_OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) $< $(CORE_OBJS) $(STUB_OBJ) \
	    $(WRAP) $(LDFLAGS) $(CMOCKA) \
	    -o $@

# Clean up test artifacts
clean:
	rm -rf $(BINDIR) coverage.info

$(BINDIR):
	mkdir -p $@
