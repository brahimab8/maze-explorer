# test/Makefile

CC       := gcc
CFLAGS   := -std=gnu11 -Wall -Wextra -g -I../include
CMOCKA   := -lcmocka
BINDIR   := bin

# build the one stubs.o
STUB_OBJ := $(BINDIR)/stubs.o
$(STUB_OBJ): stubs.c | $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# grab everything in ../build except main.o
CORE_OBJS := $(filter-out ../build/main.o,$(wildcard ../build/*.o))

# wrap the stubs we always need
WRAP := -Wl,--wrap=run_menu   \
        -Wl,--wrap=run_load_menu \
        -Wl,--wrap=load_game_slot \
        -Wl,--wrap=save_game_slot \
        -Wl,--wrap=run_settings_menu \
        -Wl,--wrap=input_init \
        -Wl,--wrap=input_poll \
        -Wl,--wrap=input_get_line \
        -Wl,--wrap=free_maze

# list of all tests that exercise game_play
GAME_PLAY_TESTS := \
    game_init_cmocka \
    game_loop_cmocka \
    game_menu_cmocka \
    maze_cmocka \
    settings_cmocka \
    timer_cmocka

# for those tests, wrap out draw_maze
$(addprefix $(BINDIR)/,$(GAME_PLAY_TESTS)): WRAP += -Wl,--wrap=draw_maze

# for the game_loop test, wrap out game_init (to avoid divide-by-zero)
$(BINDIR)/game_loop_cmocka: WRAP += -Wl,--wrap=game_init

# auto‐discover any *_cmocka.c and map to bin/
TEST_SRCS := $(wildcard *_cmocka.c)
TEST_BINS := $(patsubst %.c,$(BINDIR)/%,$(TEST_SRCS))

.PHONY: all test clean
all: $(TEST_BINS)

test: all
	@echo "Running all CMocka tests…"
	@for t in $(TEST_BINS); do \
	  echo "==> $$t"; \
	  $$t || exit 1; \
	done
	@echo "All tests passed!"

clean:
	rm -rf $(BINDIR)

# pattern‐rule for every test driver; pulls in CORE_OBJS and the right WRAP
$(BINDIR)/%: %.c $(STUB_OBJ) $(CORE_OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) \
	    $< $(STUB_OBJ) $(CORE_OBJS) \
	    $(WRAP) $(CMOCKA) \
		-o $@

$(BINDIR):
	mkdir -p $@
