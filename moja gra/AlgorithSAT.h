#ifndef ALGORITHSAT_H
#define ALGORITHSAT_H

#include <SDL.h>

//okreslanie funkcji kodu SAT opis tych funkcji w pliku SAT.cpp

bool pointInRect(const SDL_FPoint& point, const SDL_FRect& rect);
bool linesIntersect(SDL_FPoint p1, SDL_FPoint p2, SDL_FPoint q1, SDL_FPoint q2);
bool rotatedRectCollides(const SDL_FPoint* corners, const SDL_FRect& rect);

#endif