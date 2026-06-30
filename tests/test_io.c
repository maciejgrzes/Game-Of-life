#include <assert.h>
#include <stdio.h>
#include "logic.h"
#include "io.h"

#define RUN_TEST(test) printf("%s\n", #test); test(); printf("Test passed!\n");
#define TEST_SAVE_PATH "build/tmp_save.gol"

static void test_save(void) {
    Grid* g = grid_create(3, 3);
    g->cells[1][1] = 1;
    g->cells[2][2] = 1;

    assert(io_save(TEST_SAVE_PATH, g) == 0);
    grid_destroy(g);
}

static void test_load(void) {
    Grid* loaded = grid_create(3, 3);
    assert(io_load(TEST_SAVE_PATH, loaded) == 0);
    assert(loaded->cells[1][1] == 1);
    assert(loaded->cells[2][2] == 1);

    assert(io_load("tests/does_not_exist.gol", loaded) == -1);
    grid_destroy(loaded);
}

int main() {
    printf("Testing I/O units\n");

    RUN_TEST(test_save);
    RUN_TEST(test_load);
    return 0;
}
