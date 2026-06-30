#include "logic.h"
#include "io.h"
#include <stdlib.h>
#include <string.h>

// Allokowanie w pamięci siatki
Grid* grid_create(int rows, int cols) {
    Grid* g = malloc(sizeof(Grid));
    if (!g) {
        io_log("ERROR", "Błąd w alokowaniu pamięci dla Grid");
        return NULL;
    } 

    g->rows = rows;
    g->cols = cols;

    g->cells = calloc(rows, sizeof(int*));
    if (!g->cells) {
        io_log("ERROR", "Błąd w alokowaniu pamięci dla Grid->cells");
        free(g); 
        return NULL; 
    }

    for (int i = 0; i < rows; i++) {
        g->cells[i] = calloc(cols, sizeof(int));
        if (!g->cells[i]) { 
            io_log("ERROR", "Błąd w alokowaniu pamięci dla Grid->cells");
            grid_destroy(g); 
            return NULL;
        }
    }
    return g;
}

// Dealokacja siatki z pamięci
void grid_destroy(Grid* g) {
    if (!g) return;

    for (int i = 0; i < g->rows; i++)
        if (g->cells[i]) {
            free(g->cells[i]);
        } else {
            continue;
        }

    free(g->cells);
    free(g);
}

// 0 inicjalizacja siatki
void grid_init(Grid* g) {
    for (int i = 0; i < g->rows; i++)
        memset(g->cells[i], 0, g->cols * sizeof(int));
}

// Liczenie sąsiadów dla danej komórki o współrzędnych i, j
int grid_neighbors(int i, int j, Grid* g) {
    int count = 0;

    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            // pomija centralną komórke bo to jej wlasnie sąsiadów liczymy
            if (di == 0 && dj == 0) continue;
            
            int ni = i + di, nj = j + dj;
            
            // sprawdza żeby nie liczyć komórek poza siatką
            if (ni >= 0 && ni < g->rows && nj >= 0 && nj < g->cols)
                count += g->cells[ni][nj];
        }
    }
    return count;
}

// Obliczanie następnego kroku symulacji według zasad conway'a
void grid_step(Grid* g) {
    // Tymczasowa siatka do zapisania następnego stanu
    Grid* next = grid_create(g->rows, g->cols);
    if (!next) return;

    for (int i = 0; i < g->rows; i++) {
        for (int j = 0; j < g->cols; j++) {
            int n = grid_neighbors(i, j, g);

            if (g->cells[i][j] && (n < 2 || n > 3)) 
                next->cells[i][j] = 0;
            else if (!g->cells[i][j] && n == 3)           
                next->cells[i][j] = 1;
            else                                           
                next->cells[i][j] = g->cells[i][j];
        }
    }

    // Skopiowanie następnego stanu z powrotem do g
    for (int i = 0; i < g->rows; i++)
        memcpy(g->cells[i], next->cells[i], g->cols * sizeof(int));

    grid_destroy(next);
}
