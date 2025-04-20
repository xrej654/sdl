#include "AlgorithSAT.h"

// Funkcja pomocnicza: sprawdza, czy punkt le¿y w prostok¹cie
bool pointInRect(const SDL_FPoint& point, const SDL_FRect& rect) {
    return point.x >= rect.x && point.x <= rect.x + rect.w &&
        point.y >= rect.y && point.y <= rect.y + rect.h;
}

// Funkcja pomocnicza: sprawdza, czy dwie linie siê przecinaj¹
bool linesIntersect(SDL_FPoint p1, SDL_FPoint p2, SDL_FPoint q1, SDL_FPoint q2) {
    auto cross = [](SDL_FPoint a, SDL_FPoint b) {
        return a.x * b.y - a.y * b.x;
        };

    SDL_FPoint r = { p2.x - p1.x, p2.y - p1.y };
    SDL_FPoint s = { q2.x - q1.x, q2.y - q1.y };

    float denom = cross(r, s);
    if (denom == 0.0f) return false;

    SDL_FPoint qp = { q1.x - p1.x, q1.y - p1.y };
    float t = cross(qp, s) / denom;
    float u = cross(qp, r) / denom;

    return t >= 0 && t <= 1 && u >= 0 && u <= 1;
}

// Sprawdza kolizjê miêdzy obróconym prostok¹tem a zwyk³ym prostok¹tem
bool rotatedRectCollides(const SDL_FPoint* corners, const SDL_FRect& rect) {
    for (int i = 0; i < 4; ++i) {
        if (pointInRect(corners[i], rect)) return true;
    }

    SDL_FPoint wallCorners[4] = {
        { (float)rect.x, (float)rect.y },
        { (float)(rect.x + rect.w), (float)rect.y },
        { (float)(rect.x + rect.w), (float)(rect.y + rect.h) },
        { (float)rect.x, (float)(rect.y + rect.h) }
    };

    for (int i = 0; i < 4; ++i) {
        if (pointInRect(wallCorners[i], SDL_FRect{ (float)corners[0].x, (float)corners[0].y, 1,1 })) return true;
    }

    for (int i = 0; i < 4; ++i) {
        SDL_FPoint a1 = corners[i];
        SDL_FPoint a2 = corners[(i + 1) % 4];

        for (int j = 0; j < 4; ++j) {
            SDL_FPoint b1 = wallCorners[j];
            SDL_FPoint b2 = wallCorners[(j + 1) % 4];

            if (linesIntersect(a1, a2, b1, b2)) return true;
        }
    }

    return false;
}
