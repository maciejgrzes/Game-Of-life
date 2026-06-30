#include "ui.h"
#include "config.h"

// Rysowanie komórek na podstawie ich stanu (żywa / martwa)
static void draw_cell(SDL_Surface* s, int cx, int cy, Uint32 color) {
    SDL_Rect r = { cx * CELL_SIZE, cy * CELL_SIZE, CELL_SIZE, CELL_SIZE };
    SDL_FillSurfaceRect(s, &r, color);
}

// Przejście przez wszystkie komórki i ich narysowanie
void ui_draw_matrix(SDL_Surface* s, Grid* g) {
    for (int i = 0; i < g->rows; i++)
        for (int j = 0; j < g->cols; j++)
            draw_cell(s, j, i, g->cells[i][j] ? COLOR_CELL : COLOR_BG);
}

// Rysowanie linii siatki
void ui_draw_grid(SDL_Surface* s) {
    for (int x = CELL_SIZE; x < WINDOW_W; x += CELL_SIZE) {
        SDL_Rect col = { x, 0, GRID_LINE_W, WINDOW_H };
        SDL_FillSurfaceRect(s, &col, COLOR_GRID);
    }
    for (int y = CELL_SIZE; y < WINDOW_H; y += CELL_SIZE) {
        SDL_Rect row = { 0, y, WINDOW_W, GRID_LINE_W };
        SDL_FillSurfaceRect(s, &row, COLOR_GRID);
    }
}

// Czyszczenie siatki / ustawienie wszystkich komórek jako martwe
void ui_clear(SDL_Surface* s, Grid* g) {
    grid_init(g);
    ui_draw_matrix(s, g);
    ui_draw_grid(s);
}

// Rysowanie komórek myszą
void ui_paint_cell(SDL_Surface* s, float mx, float my, Grid* g, int alive) {
    if (mx < 0 || my < 0) return;

    int r = (int)my / CELL_SIZE;
    int c = (int)mx / CELL_SIZE;
    if (r >= 0 && r < g->rows && c >= 0 && c < g->cols) {
        g->cells[r][c] = alive;
        draw_cell(s, c, r, alive ? COLOR_CELL : COLOR_BG);
    }
}
