#ifndef LOGIC_H
#define LOGIC_H

// Główna struktura siatki
typedef struct {
    int**  cells;
    int    rows;
    int    cols;
} Grid;

Grid* grid_create(int rows, int cols);
void grid_destroy(Grid* g);
void grid_init(Grid* g);
void grid_step(Grid* g);
int  grid_neighbors(int i, int j, Grid* g);

#endif
