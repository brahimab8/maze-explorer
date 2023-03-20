# Maze Explorer

Maze Explorer is a terminal‑based C application: navigate a procedurally generated maze, collect extra projectiles, and fight/evade roaming monsters.

## Roadmap

1. Maze Generation  
2. Player Input & Rendering  
3. Projectile & Pickup System  
4. Monster Movement  
5. Level Progression & Save/Load  
6. Timing & Best‑time Tracking
7. Persistent Configuration 
8. Unit Testing & Coverage

---

## Technical Overview

- **Language**: C (ISO C11)  
- **Build**: Make, GCC/Clang  
- **Maze**: Recursive backtracker  
- **I/O**: Console & file  
- **Timing**: Sub‑second precision  
- **Tests**: CMocka  
- **Coverage**: lcov + genhtml via `make coverage`

---

## Getting Started

On a Linux system with gcc and make installed, first clone the repository, then build and run the game:

```bash
make
./maze_explorer
```

## Running Tests
Unit tests are written with CMocka. To build and run them:

```bash
make test
```

## Generating Coverage Reports
This will rebuild both the library and tests with coverage instrumentation, run all tests, then generate an HTML report in coverage-report/:

```bash
make coverage
```

Open coverage-report/index.html in the browser to inspect the coverage results.

---

## License

This project is released under the [MIT License](LICENSE).

