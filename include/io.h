#ifndef IO_H
#define IO_H

#include "logic.h"
#include <SDL3/SDL.h>

void io_save_dialog(SDL_Window* window, Grid* g);
void io_load_dialog(SDL_Window* window, SDL_Surface* surface, Grid* g);
int io_load(const char* path, Grid* g);
int io_save(const char* path, Grid* g);


int io_log_open(const char* path);
void io_log_close(void);
void io_log(const char* type ,const char* message);

#endif
