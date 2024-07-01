#include <SDL2/SDL.h>
#include "line.hpp"

line::line(int x1_, int y1_, int x2_, int y2_)
{
    x1 = x1_;
    y1 = y1_;
    x2 = x2_;
    y2 = y2_;
    vector_x_component = x2 - x1;
    vector_y_component = y2 - y1; 
}

line::draw(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}