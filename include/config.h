#ifndef CONFIG_H
#define CONFIG_H

// Wymiary okna
#define WINDOW_W    1700
#define WINDOW_H    900

// Kolory
#define COLOR_BG    0x000000
#define COLOR_CELL  0xffffff
#define COLOR_GRID  0x111111

// Siatka
#define CELL_SIZE   20
#define GRID_LINE_W 2
#define COLUMNS     (WINDOW_W / CELL_SIZE)
#define ROWS        (WINDOW_H / CELL_SIZE)

// Szybkość symulacji (ms)
#define SPEED_DEFAULT  100
#define SPEED_MIN        5
#define SPEED_MAX     1000
#define SPEED_STEP       5

#endif
