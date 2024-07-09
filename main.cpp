#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <limits>
#include "line.hpp"

const double PI = 3.14159265358979;
const int WIDTH = 1000;
const int HEIGHT = 600;
const unsigned int rayLenght = 200;

void initizlizeRays(std::vector<line> &lineRays, unsigned int *linesCount, int *mouse_x, int *mouse_y);
void drawRays(SDL_Renderer* renderer, const std::vector<line> lineRays);
void drawWalls(SDL_Renderer* renderer, const std::vector<line> walls);
void drawScene(SDL_Renderer* renderer, const std::vector<line> lineRays, const std::vector<line> walls);
void detectCollision(SDL_Renderer* renderer, std::vector<line> &lineRays, std::vector<line> &walls);
bool areParallel(const line ray, const line wall);

int main(int argc, char* argv[]) 
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Ray casting simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    if(window == NULL) 
    {
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    unsigned int linesCount = 360;
    std::vector<line> rayLines;

    unsigned int wall_n = 15;
    std::vector<line> walls;
    int w_x1, w_x2, w_y1, w_y2;
    for(int i = 0; i < wall_n; i++)
    {
        w_x1 = std::rand() % (WIDTH + 1);
        w_y1 = std::rand() % (HEIGHT + 1);
        w_x2 = std::rand() % (WIDTH + 1);
        w_y2 = std::rand() % (HEIGHT + 1);
        walls.push_back(line(w_x1, w_x2, w_y1, w_y2));  
    }

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
    for(int i = 0; i < (*linesCount); i++)
    {
        //if((i * step) % 90 != 0) // n * 90, n being 1,2,3,... are buggy
        lineRays.push_back(line(*mouse_x, *mouse_y, (*mouse_x) + rayLenght * cos(i * step * PI / 180), (*mouse_y) + rayLenght * sin(i * step * PI / 180)));
    }
}

void drawRays(SDL_Renderer* renderer, const std::vector<line> lineRays)
{
    SDL_Color rayColor;
    rayColor.r = 255;
    rayColor.g = 255;
    rayColor.b = 100;
    rayColor.a = 100;

    for(line l: lineRays)
    {
        l.draw(renderer, rayColor);
    }
}

void drawWalls(SDL_Renderer* renderer, const std::vector<line> walls)
{
    SDL_Color wallColor;
    wallColor.r = 255;
    wallColor.g = 255;
    wallColor.b = 255;
    wallColor.a = 255;
    for(line l: walls)
    {
        l.draw(renderer, wallColor);
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
    int intersect_x, intersect_y;
    float minDistance = std::numeric_limits<float>::max();
    float currentDistance = 0.0f;
    int min_x, min_y;
    bool isIntersected = false;
    std::vector<line> removeRays;
    int lineIndex = 0;

    for(line ray : lineRays)
    {    
        for(line wall : walls)
        {
            // intersection formula
            t_w = (float)(wall.y2 * ray.vector_x_component - ray.y2 * ray.vector_x_component - wall.x2 * ray.vector_y_component + ray.x2 * ray.vector_y_component) /
                                (float)(wall.vector_x_component * ray.vector_y_component - wall.vector_y_component * ray.vector_x_component);
            intersect_x = std::ceil(wall.x2 + t_w * wall.vector_x_component);
            intersect_y = std::ceil(wall.y2 + t_w * wall.vector_y_component);
            
            // if point intersect i remvoe the old ray and add the new one
            currentDistance = (ray.x1 - intersect_x) * (ray.x1 - intersect_x) + (ray.y1 - intersect_y) * (ray.y1 - intersect_y);

            if(currentDistance < minDistance && wall.isPointOnLine(intersect_x, intersect_y) && ray.isPointOnLine(intersect_x, intersect_y))
            {
                minDistance = currentDistance;
                min_x = intersect_x;
                min_y = intersect_y;
                isIntersected = true;
            }
        }
        
        if(isIntersected)
        {
            lineRays.push_back(line(ray.x1, ray.y1, min_x, min_y));
            //lineRays.insert(lineRays.begin() + lineIndex, line(ray.x1, ray.y1, min_x, min_y));
            removeRays.push_back(ray);
        }
        isIntersected = false;
        minDistance = std::numeric_limits<float>::max();
        currentDistance = 0.0f;

        lineIndex++;
    }

    // checking the remove list and then removing in the actual ray vector
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