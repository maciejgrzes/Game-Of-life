#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>
#include "config.h"
#include "io.h"
#include "logic.h"
#include "ui.h"

int main(void) {
    if (io_log_open("logs/app.log") != 0) {
        char* msg = "Błąd w otworzeniu pliku logs/app.log";
        printf("%s", msg);
        return -1;
    } io_log("INFO", "Aplikacja uruchomiona");

    // Inicjalizacja okna i pozostałych zmiennych
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        char* msg = "Błąd w inicjalizacji okna (SDL_Init)";
        printf("%s", msg);
        io_log("ERROR", msg);
        io_log_close();
        return -1;
    }

    SDL_Window*  window  = SDL_CreateWindow("Game Of Life", WINDOW_W, WINDOW_H, 0);
    if (!window) {
        char* msg = "Błąd w inicjalizacji okna (SDL_CreateWindow)";
        printf("%s", msg);
        io_log("ERROR", msg);
        io_log_close();
        SDL_Quit();
        return -1;
    }

    SDL_Surface* surface = SDL_GetWindowSurface(window);
    if (!surface) {
        char* msg = "Błąd w inicjalizacji okna (SDL_Surface)";
        printf("%s", msg);
        io_log("ERROR", msg);
        io_log_close();
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }


    Grid* grid = grid_create(ROWS, COLUMNS);
    if (!grid) {
        char* msg = "Błąd w inicjalizacji siatki (grid_create)";
        printf("%s", msg);
        io_log("ERROR", msg);
        io_log_close();
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    grid_init(grid);
    ui_draw_matrix(surface, grid);
    ui_draw_grid(surface);

    SDL_Event event;
    float x = 0, y = 0;
    int mouseLeft = 0, mouseRight = 0;
    int paused = 1, running = 1, speed = SPEED_DEFAULT;

    // Główna pętla gry
    while (running) {
        // Sprawdzanie danych wejściowych od użytkownika
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EVENT_QUIT:
                running = 0; break;

            case SDL_EVENT_KEY_DOWN:
                switch (event.key.key) {
                    case SDLK_ESCAPE: 
                        running = 0; 
                        break;
                    case SDLK_SPACE: 
                        paused = !paused; 
                        break;
                    // Czyszczenie macierzy
                    case SDLK_BACKSPACE: 
                        ui_clear(surface, grid); 
                        break;
                    case SDLK_UP: 
                        if (speed > SPEED_MIN) speed -= SPEED_STEP; 
                        break;
                    case SDLK_DOWN: 
                        if (speed < SPEED_MAX) speed += SPEED_STEP; 
                        break;
                    // Zapisywanie / wczytywanie stanów gry z plików
                    case SDLK_S: 
                        paused = 1;
                        io_save_dialog(window, grid); 
                        break;
                    case SDLK_L: 
                        paused = 1;
                        io_load_dialog(window, surface, grid); 
                        break;
                }
                break;

            // Rysowanie myszą
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (event.button.button == SDL_BUTTON_LEFT)  { mouseLeft  = 1; paused = 1; }
                if (event.button.button == SDL_BUTTON_RIGHT) { mouseRight = 1; }
                break;

            case SDL_EVENT_MOUSE_BUTTON_UP:
                if (event.button.button == SDL_BUTTON_LEFT)  mouseLeft  = 0;
                if (event.button.button == SDL_BUTTON_RIGHT) mouseRight = 0;
                break;

            case SDL_EVENT_MOUSE_MOTION:
                SDL_GetMouseState(&x, &y);
                if (mouseLeft)  { ui_paint_cell(surface, x, y, grid, 1); ui_draw_grid(surface); }
                if (mouseRight) { ui_paint_cell(surface, x, y, grid, 0); ui_draw_grid(surface); }
                break;
            }
            SDL_UpdateWindowSurface(window);
        }

        if (!paused) {
            grid_step(grid);
            ui_draw_matrix(surface, grid);
            ui_draw_grid(surface);
            SDL_UpdateWindowSurface(window);
            SDL_Delay(speed);
        }
    }
    // Logi + czyszczenie przydzielonej pamięci + zamykanie okna
    io_log("INFO", "Aplikacja zamknięta");
    io_log_close();
    grid_destroy(grid);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
