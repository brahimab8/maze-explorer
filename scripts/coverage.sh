#!/usr/bin/env bash
set -euo pipefail

# === CONFIG ===
# linker wrap flags to redirect real game/menu calls into stubs
WRAP_FLAGS="-Wl,--wrap=run_menu \
            -Wl,--wrap=run_load_menu \
            -Wl,--wrap=load_game_slot \
            -Wl,--wrap=save_game_slot \
            -Wl,--wrap=run_settings_menu \
            -Wl,--wrap=input_init \
            -Wl,--wrap=input_poll \
            -Wl,--wrap=input_get_line \
            -Wl,--wrap=free_maze"
CMOCKA_LIB="-lcmocka"

# Clean previous coverage build & reports
rm -rf build-coverage coverage.info coverage-report run_* build-coverage

# Create coverage build directory
mkdir -p build-coverage

# Compile & instrument all library sources (except main.c)
for src in src/*.c; do
  fname=$(basename "$src")
  mod="${fname%.c}"
  if [ "$mod" != "main" ]; then
    gcc -std=gnu11 -Wall -Wextra -g --coverage \
        -Iinclude -c "$src" \
        -o build-coverage/"$mod".o
  fi
done

# Compile & instrument the POSIX UI
if [ -f src/ui_posix.c ]; then
  gcc -std=gnu11 -Wall -Wextra -g --coverage \
      -Iinclude -c src/ui_posix.c \
      -o build-coverage/ui_posix.o
fi

# Compile & instrument test stubs.c
if [ -f test/stubs.c ]; then
  gcc -std=gnu11 -Wall -Wextra -g --coverage \
      -Iinclude -Itest -c test/stubs.c \
      -o build-coverage/stubs.o
fi

# Compile & instrument each CMocka test driver
for t in test/*_cmocka.c; do
  fname=$(basename "$t")
  mod="${fname%.c}"
  gcc -std=gnu11 -Wall -Wextra -g --coverage \
      -Iinclude -Itest -c "$t" \
      -o build-coverage/"$mod".o
done

# Gather library & stub objects (exclude test driver objects)
LIBOBJS=$(find build-coverage -maxdepth 1 -name '*.o' ! -name '*_cmocka.o' -printf 'build-coverage/%f ')

# Link & run each test binary individually (with wrap flags)
for t in test/*_cmocka.c; do
  mod="${t##*/}"
  mod="${mod%.c}"
  echo "==> Running $mod"
  gcc -std=gnu11 -Wall -Wextra -g --coverage $WRAP_FLAGS \
      $LIBOBJS build-coverage/"$mod".o $CMOCKA_LIB \
      -o run_$mod
  ./run_$mod
done

# Capture coverage data
echo "Capturing coverage data..."
lcov --capture --directory . --output-file coverage.info

# Strip out system files and test code
echo "Filtering out system and test code..."
lcov --remove coverage.info '/usr/*' '*/test/*' --output-file coverage.info

# Generate HTML report
echo "Generating HTML report..."
genhtml coverage.info --output-directory coverage-report

echo
echo ">> Coverage report ready: coverage-report/index.html"
