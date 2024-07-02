#ifndef LINE_H
#define LINE_H

#include <SDL2/SDL.h>

class line
{
    public:
        int x1, y1, x2, y2;
        int vector_x_component;
        int vector_y_component;    

        line(int x1_, int y1_, int x2_, int y2_);
        draw(SDL_Renderer* renderer);
        compare(const line &other);
        isPointOnLine(int point_x, int point_y);
        isLongEnough(int point_x, int point_y);
};

#endif // LINE_H