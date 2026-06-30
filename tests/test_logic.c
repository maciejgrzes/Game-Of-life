#include <assert.h>
#include <stdio.h>
#include "logic.h"

#define RUN_TEST(test) printf("%s\n", #test); test(); printf("Test passed!\n");


void io_log(const char* type, const char* message) {
    (void)type;
    (void)message;
}

static void assert_grid_equals(Grid* g, const int* expected) {
    for (int i = 0; i < g->rows; i++) {
        for (int j = 0; j < g->cols; j++) {
            if (g->cells[i][j] != expected[i * g->cols + j]) {
                fprintf(stderr, "Cell [%d][%d]: expected %d, got %d\n",
                        i, j, expected[i * g->cols + j], g->cells[i][j]);
            }
            assert(g->cells[i][j] == expected[i * g->cols + j]);
        }
    }
}

static void test_grid_create(void) {
    Grid* g = grid_create(3, 3);

    assert(g != NULL);
    assert(g->rows == 3);
    assert(g->cols == 3);

    for (int i = 0; i < g->rows; i++) {
      for (int j = 0; j < g->cols; j++) {
          assert(g->cells[i][j] == 0);
      }
    }

    grid_destroy(g);
}


static void test_neighbors(void) {
    Grid* g = grid_create(3, 3);

    g->cells[0][0] = 1;
    g->cells[0][1] = 1;
    g->cells[1][0] = 1;

    assert(grid_neighbors(1, 1, g) == 3);
    assert(grid_neighbors(0, 0, g) == 2);

    grid_destroy(g);
}


static void test_grid_step_blinker(void) {
    Grid* g = grid_create(5, 5);
    const int expected[] = {
        0, 0, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 0, 0, 0
    };

    g->cells[2][1] = 1;
    g->cells[2][2] = 1;
    g->cells[2][3] = 1;

    grid_step(g);

    assert_grid_equals(g, expected);

    grid_destroy(g);
}


static void test_grid_init_clears_cells(void) {
    Grid* g = grid_create(2, 2);

    g->cells[0][0] = 1;
    g->cells[0][1] = 1;
    g->cells[1][0] = 1;

    grid_init(g);

    assert(g->cells[0][0] == 0);
    assert(g->cells[0][1] == 0);
    assert(g->cells[1][0] == 0);
    assert(g->cells[1][1] == 0);

    grid_destroy(g);
}

static void test_grid_step_block_stays_alive(void) {
    Grid* g = grid_create(4, 4);
    const int expected[] = {
        0, 0, 0, 0,
        0, 1, 1, 0,
        0, 1, 1, 0,
        0, 0, 0, 0
    };

    g->cells[1][1] = 1;
    g->cells[1][2] = 1;
    g->cells[2][1] = 1;
    g->cells[2][2] = 1;

    grid_step(g);

    assert_grid_equals(g, expected);

    grid_destroy(g);
}

static void test_grid_step_lonely_cell_dies(void) {
    Grid* g = grid_create(3, 3);

    g->cells[1][1] = 1;

    grid_step(g);

    assert(g->cells[1][1] == 0);

    grid_destroy(g);
}

static void test_grid_step_birth_with_three_neighbors(void) {
    Grid* g = grid_create(3, 3);
    const int expected[] = {
        0, 0, 0,
        0, 1, 0,
        0, 0, 0
    };

    g->cells[0][0] = 1;
    g->cells[0][2] = 1;
    g->cells[2][0] = 1;

    grid_step(g);

    assert_grid_equals(g, expected);

    grid_destroy(g);
}

static void test_grid_step_overpopulated_cell_dies(void) {
    Grid* g = grid_create(3, 3);

    g->cells[1][1] = 1;
    g->cells[0][0] = 1;
    g->cells[0][1] = 1;
    g->cells[0][2] = 1;
    g->cells[1][0] = 1;

    grid_step(g);

    assert(g->cells[1][1] == 0);

    grid_destroy(g);
}

static void test_neighbors_corner_does_not_wrap(void) {
    Grid* g = grid_create(3, 3);

    g->cells[0][1] = 1;
    g->cells[1][0] = 1;
    g->cells[1][1] = 1;
    g->cells[2][2] = 1;

    assert(grid_neighbors(0, 0, g) == 3);

    grid_destroy(g);
}

int main(void) {
    printf("Testing logic units\n");

    RUN_TEST(test_grid_create);
    RUN_TEST(test_grid_init_clears_cells);
    RUN_TEST(test_neighbors);
    RUN_TEST(test_neighbors_corner_does_not_wrap);
    RUN_TEST(test_grid_step_blinker);
    RUN_TEST(test_grid_step_block_stays_alive);
    RUN_TEST(test_grid_step_lonely_cell_dies);
    RUN_TEST(test_grid_step_birth_with_three_neighbors);
    RUN_TEST(test_grid_step_overpopulated_cell_dies);

    return 0;
}
