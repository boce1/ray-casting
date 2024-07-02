#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include <vector>
#include "line.hpp"

const double PI = 3.14159;
const int WIDTH = 1000;
const int HEIGHT = 600;

void initizlizeRays(std::vector<line> &lineRays, unsigned int *linesCount, int *mouse_x, int *mouse_y);
void drawRays(SDL_Renderer* renderer, const std::vector<line> lineRays);
void drawWalls(SDL_Renderer* renderer, const std::vector<line> walls);
void drawScene(SDL_Renderer* renderer, const std::vector<line> lineRays, const std::vector<line> walls);
void detectCollision(SDL_Renderer* renderer, std::vector<line> &lineRays, std::vector<line> &walls);
bool areParallel(const line ray, const line wall);

int main(int argc, char* argv[]) 
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Pendulum", 100, 100, WIDTH, HEIGHT, 0);
    if(window == NULL) 
    {
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    unsigned int linesCount = 360;
    std::vector<line> rayLines;

    std::vector<line> walls; 
    line wall1(100,100,200,200);
    walls.push_back(wall1);

    int mouse_x, mouse_y;
    bool running = true;
    SDL_Event event;
    Uint64 nowTicks = SDL_GetPerformanceCounter();
    Uint64 lastTicks = 0;
    double deltaTime = 0; 
    while(running)
    {
        lastTicks = nowTicks;
        nowTicks = SDL_GetPerformanceCounter();
        deltaTime = (double)((nowTicks - lastTicks)*1000 / (double)SDL_GetPerformanceFrequency());

        SDL_GetMouseState(&mouse_x, &mouse_y);
        initizlizeRays(rayLines, &linesCount, &mouse_x, &mouse_y);
        detectCollision(renderer, rayLines, walls);
        drawScene(renderer, rayLines, walls);
        rayLines.clear();
    
        if(SDL_PollEvent(&event)) 
        {
            if(event.type == SDL_QUIT)
            {
                running = false;
            }
        }
        
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void initizlizeRays(std::vector<line> &lineRays, unsigned int *linesCount, int *mouse_x, int *mouse_y)
{
    int step = 360 / (*linesCount);
    unsigned int rayLenght = 250;
    for(int i = 0; i < (*linesCount); i++)
    {
        lineRays.push_back(line(*mouse_x, *mouse_y, (*mouse_x) + rayLenght * cos(i * step * PI / 180), (*mouse_y) + rayLenght * sin(i * step * PI / 180)));
    }
}

void drawRays(SDL_Renderer* renderer, const std::vector<line> lineRays)
{
    for(line l: lineRays)
    {
        l.draw(renderer);
    }
}

void drawWalls(SDL_Renderer* renderer, const std::vector<line> walls)
{
    for(line l: walls)
    {
        l.draw(renderer);
    }
}

void drawScene(SDL_Renderer* renderer, const std::vector<line> lineRays, const std::vector<line> walls)
{
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);
    drawRays(renderer, lineRays);
    drawWalls(renderer, walls);
    SDL_RenderPresent(renderer);
}

void detectCollision(SDL_Renderer* renderer, std::vector<line> &lineRays, std::vector<line> &walls)
{
    float t_w;
    int new_x, new_y;
    std::vector<line> removeRays; 
    for(line ray : lineRays)
    {
        for(line wall : walls)
        {
            t_w = (float)(wall.y2 * ray.vector_x_component - ray.y2 * ray.vector_x_component - wall.x2 * ray.vector_y_component + ray.x2 * ray.vector_y_component) /
                                (float)(wall.vector_x_component * ray.vector_y_component - wall.vector_y_component * ray.vector_x_component);
            new_x = (int)(wall.x2 + t_w * wall.vector_x_component);
            new_y = (int)(wall.y2 + t_w * wall.vector_y_component);
            
            if(wall.isPointOnLine(new_x, new_y) && 
                    ray.isPointOnLine(new_x, new_y) &&
                    ray.isLongEnough(new_x, new_y))
            {
                lineRays.push_back(line(ray.x1, ray.y1, new_x, new_y));
                removeRays.push_back(line(ray.x1, ray.y1, ray.x2, ray.y2));
            }
        }
    }
    for(line removeRay : removeRays)
    {
        for(int i = 0; i < lineRays.size(); i++)
        {
            if(removeRay.compare(lineRays[i]))
            {
                lineRays.erase(lineRays.begin() + i);
                break;
            }
        }
    }
    removeRays.clear();

}