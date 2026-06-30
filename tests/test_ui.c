#include <assert.h>
#include <stdio.h>
#include "ui.h"
#include "logic.h"
#include "config.h"

#define RUN_TEST(test) printf("%s\n", #test); test(); printf("Test passed!\n");

void io_log(const char* type, const char* message) {
    (void)type;
    (void)message;
}

static void test_paint_cell_sets_alive(void) {
    Grid* g = grid_create(ROWS, COLUMNS);
    SDL_Surface* surface = SDL_CreateSurface(WINDOW_W, WINDOW_H, SDL_PIXELFORMAT_RGBA8888);

    ui_paint_cell(surface, 25, 25, g, 1);

    assert(g->cells[1][1] == 1);

    SDL_DestroySurface(surface);
    grid_destroy(g);
}

static void test_paint_cell_sets_dead(void) {
    Grid* g = grid_create(ROWS, COLUMNS);
    SDL_Surface* surface = SDL_CreateSurface(WINDOW_W, WINDOW_H, SDL_PIXELFORMAT_RGBA8888);

    g->cells[1][1] = 1;

    ui_paint_cell(surface, 25, 25, g, 0);

    assert(g->cells[1][1] == 0);

    SDL_DestroySurface(surface);
    grid_destroy(g);
}

static void test_paint_outside_grid_does_nothing(void) {
    Grid* g = grid_create(ROWS, COLUMNS);
    SDL_Surface* surface = SDL_CreateSurface(WINDOW_W, WINDOW_H, SDL_PIXELFORMAT_RGBA8888);

    ui_paint_cell(surface, -10, -10, g, 1);
    ui_paint_cell(surface, WINDOW_W + 100, WINDOW_H + 100, g, 1);

    for (int i = 0; i < g->rows; i++) {
        for (int j = 0; j < g->cols; j++) {
            assert(g->cells[i][j] == 0);
        }
    }

    SDL_DestroySurface(surface);
    grid_destroy(g);
}

static void test_clear_resets_grid(void) {
    Grid* g = grid_create(ROWS, COLUMNS);
    SDL_Surface* surface = SDL_CreateSurface(WINDOW_W, WINDOW_H, SDL_PIXELFORMAT_RGBA8888);

    g->cells[0][0] = 1;
    g->cells[5][5] = 1;
    g->cells[10][10] = 1;

    ui_clear(surface, g);

    for (int i = 0; i < g->rows; i++) {
        for (int j = 0; j < g->cols; j++) {
            assert(g->cells[i][j] == 0);
        }
    }

    SDL_DestroySurface(surface);
    grid_destroy(g);
}

int main() {
    printf("Testing ui units\n");

    RUN_TEST(test_paint_cell_sets_alive);
    RUN_TEST(test_paint_cell_sets_dead);
    RUN_TEST(test_paint_outside_grid_does_nothing);
    RUN_TEST(test_clear_resets_grid);
    return 0;
}
