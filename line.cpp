#include <SDL2/SDL.h>
#include <cmath>
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

line::compare(const line &other)
{   
    return (x1 == other.x1 &&
            x2 == other.x2 &&
            y1 == other.y1 &&
            y2 == other.y2 &&
            vector_x_component == other.vector_x_component &&
            vector_y_component == other.vector_y_component);
}

line::isPointOnLine(int point_x, int point_y)
{
    bool x_factor = false; 
    bool y_factor = false;
    if(vector_x_component > 0 && x1 <= point_x && point_x <= x2)
    {
        x_factor = true;
    }
    if(vector_x_component < 0 && x2 <= point_x && point_x <= x1)
    {
        x_factor = true;
    }
    if(vector_y_component > 0 && y1 <= point_y && point_y <= y2)
    {
        y_factor = true;
    }
    if(vector_y_component < 0 && y2 <= point_y && point_y <= y1)
    {
        y_factor = true;
    }

    return x_factor && y_factor;
}


line::isLongEnough(int point_x, int point_y)
{
    return std::fabs(vector_x_component) >= std::fabs(point_x - x1) && 
            std::fabs(vector_y_component) >= std::fabs(point_y - y1);
}