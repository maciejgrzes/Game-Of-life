#ifndef UI_H
#define UI_H

#include <SDL3/SDL.h>
#include "logic.h"

void ui_draw_matrix(SDL_Surface* s, Grid* g);
void ui_draw_grid(SDL_Surface* s);
void ui_clear(SDL_Surface* s, Grid* g);
void ui_paint_cell(SDL_Surface* s, float mx, float my, Grid* g, int alive);

#endif
