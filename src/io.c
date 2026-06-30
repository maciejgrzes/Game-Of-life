#include "io.h"
#include "ui.h"
#include "logic.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include <time.h>

// SDL3 wywołuje to po potwierdzeniu/anulowaniu okna dialogowego przez użytkownika
static Grid* s_pending_grid = NULL;
static SDL_Surface* s_pending_surface = NULL;

// Otwieranie pliku logów
static FILE* log = NULL;

// Zapisywanie stanu symulacji do pliku
int io_save(const char* path, Grid* g) {
    FILE* f = fopen(path, "w");
    if (!f) {
        char msg[512];
        snprintf(msg, sizeof(msg), "Błąd w zapisie stanu gry %s", path);
        io_log("ERROR", msg);
        return -1;
    }

    const char* error_source = NULL;

    if (fprintf(f, "ROWS=%d COLS=%d\n", g->rows, g->cols) < 0)
        error_source = "fprintf";

    for (int i = 0; !error_source && i < g->rows; i++) {
        for (int j = 0; !error_source && j < g->cols; j++) {
            if (fputc(g->cells[i][j] ? '1' : '0', f) == EOF) {
                error_source = "fputc";
            }
        }

        if (!error_source && fputc('\n', f) == EOF)
            error_source = "fputc";
    }

    if (fclose(f) == EOF && !error_source)
        error_source = "fclose";

    if (error_source) {
        char msg[512];
        snprintf(msg, sizeof(msg), "Błąd w zapisie stanu gry %s (%s)", path, error_source);
        printf("%s", msg);
        io_log("ERROR", msg);
        return -1;
    }

    char msg[512];
    snprintf(msg, sizeof(msg), "Plik %s stanu gry został zapisany", path);
    io_log("INFO", msg);
    return 0;
}


// Wczytywanie stanu symulacji z pliku
int io_load(const char* path, Grid* g) {
    FILE* f = fopen(path, "r");
    if (!f) { 
        char msg[512];
        snprintf(msg, sizeof(msg), "Błąd w otwieraniu pliku stanu gry %s", path);
        io_log("ERROR", msg);
        return -1;
    }
    int rows, cols;
    if (fscanf(f, "ROWS=%d COLS=%d\n", &rows, &cols) != 2 || rows != g->rows || cols != g->cols) {
        fclose(f);
        return -1;
    }

    for (int i = 0; i < g->rows; i++) {
        for (int j = 0; j < g->cols; j++) {
            int ch = fgetc(f);
            if (ch == '1') {
                g->cells[i][j] = 1;
            } else if (ch == '0') {
                g->cells[i][j] = 0;
            } else {
                char* msg = "Błąd z wczytywaniem tablicy z pliku (możliwy zły format pliku)";
                printf("%s", msg);
                io_log("ERROR", msg);
                fclose(f);
                return -1;
            }
        }
        fgetc(f);
    }
    fclose(f);
    char msg[512];
    snprintf(msg, sizeof(msg), "Plik %s stanu gry został wczytany", path);
    io_log("INFO", msg);
    return 0;
}


// Funkcje opakowujące io_save i io_load dla okna dialogowego plików SDL
static void save_callback(void* userdata, const char* const* files, int filter) {
    (void)userdata; (void)filter;
    if (files && files[0])
        if (io_save(files[0], s_pending_grid) != 0) {
            char* msg = "Błąd z wywołaniem (io_save w save_callback)";
            printf("%s", msg);
            io_log("ERROR", msg);
        }
}

static void load_callback(void* userdata, const char* const* files, int filter) {
    (void)userdata; (void)filter;
    if (files && files[0]) {
        if (io_load(files[0], s_pending_grid) == 0) {
            ui_draw_matrix(s_pending_surface, s_pending_grid);
            ui_draw_grid(s_pending_surface);
        } else {
            char* msg = "Błąd z wywołaniem (io_load w load_callback)";
            printf("%s", msg);
            io_log("ERROR", msg);
        }
    }
}

// Otwieranie systemowego menedżera plików do wyboru plików zapisu
void io_save_dialog(SDL_Window* window, Grid* g) {
    s_pending_grid = g;

    const char* base = SDL_GetBasePath();
    if (!base) {
        char* msg = "SDL_GetBasePath zwrócił NULL";
        printf("%s", msg);
        io_log("ERROR", msg);
        return;
    }
    char saves_path[512];
    snprintf(saves_path, sizeof(saves_path), "%ssaves", base);

    SDL_DialogFileFilter filters[] = {
        { "Game of Life save", "gol" },
        { "All files",         "*"   }
    };
    SDL_ShowSaveFileDialog(save_callback, NULL, window, filters, 2, saves_path);
}


void io_load_dialog(SDL_Window* window, SDL_Surface* surface, Grid* g) {
    s_pending_grid    = g;
    s_pending_surface = surface;

    const char* base = SDL_GetBasePath();
    char saves_path[512];
    snprintf(saves_path, sizeof(saves_path), "%ssaves", base);

    SDL_DialogFileFilter filters[] = {
        { "Game of Life save", "gol" },
        { "All files",         "*"   }
    };
    SDL_ShowOpenFileDialog(load_callback, NULL, window, filters, 2, saves_path, 0);
}

// Funkcje do logowania :3
int io_log_open(const char* path) {
    log = fopen(path, "a");
    if (log) {
        return 0;
    } else {
        return -1;
    }
}

void io_log_close(void) {
    if (log) fclose(log);
}

void io_log(const char* type ,const char* message) {
    if (!log) return;

    char timestamp[32];
    time_t now = time(NULL);

    struct tm* t = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S]", t);

    fprintf(log, "%s %s  %s\n", timestamp, type, message);
}
