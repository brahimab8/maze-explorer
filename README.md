# Maze Explorer

Maze Explorer is a terminal‑based C application: navigate a procedurally generated maze, collect extra projectiles, and fight/evade roaming monsters.

## Roadmap

1. Maze Generation  
2. Player Input & Rendering  
3. Projectile & Pickup System  
4. Monster Movement  
5. Level Progression & Save/Load  
6. Timing & Best‑time Tracking

---

## Technical Overview

- **Language**: C (ISO C11)  
- **Build**: Make, GCC/Clang  
- **Maze**: Recursive backtracker  
- **I/O**: Console & file  
- **Timing**: Sub‑second precision  

---

## Getting Started

On a Linux system with `gcc` and `make` installed:

```bash
git clone <repo-url>
cd maze-explorer
make
./maze_explorer
```

---

## License

This project is released under the [MIT License](LICENSE).

