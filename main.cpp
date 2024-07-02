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
    // test walls 
    line wall1(625, 903, 101, 284);
    walls.push_back(wall1);
    line wall2(203, 185, 538, 24);
    walls.push_back(wall2);
    line wall3(0, 387, 474, 593);
    walls.push_back(wall3);
    line wall4(490, 902, 502, 456);
    walls.push_back(wall4);
    line wall5(547, 168, 60, 320);
    walls.push_back(wall5);
    line wall6(489, 707, 132, 400);
    walls.push_back(wall6);
    line wall7(125, 790, 6, 554);
    walls.push_back(wall7);
    line wall8(741, 107, 509, 461);
    walls.push_back(wall8);
    line wall9(979, 161, 539, 404);
    walls.push_back(wall9);
    line wall10(621, 707, 248, 496);
    walls.push_back(wall10);
    line wall11(899, 611, 337, 265);
    walls.push_back(wall11);
    line wall12(644, 619, 90, 344);
    walls.push_back(wall12);
    line wall13(665, 991, 102, 159);
    walls.push_back(wall13);
    line wall14(271, 929, 411, 48);
    walls.push_back(wall14);
    line wall15(284, 527, 203, 215);
    walls.push_back(wall15);
    line wall16(586, 620, 367, 117);
    walls.push_back(wall16);
    line wall17(830, 541, 513, 391);
    walls.push_back(wall17);
    line wall18(379, 580, 326, 263);
    walls.push_back(wall18);
    line wall19(306, 943, 99, 478);
    walls.push_back(wall19);
    line wall20(43, 506, 152, 105);
    walls.push_back(wall20);
    // test walls

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
    int intersect_x, intersect_y;
    float minDistance = 99999999.0f;
    float currentDistance = 0.0f;
    int min_x, min_y;
    bool isIntersected = false;
    std::vector<line> removeRays; 
    for(line ray : lineRays)
    {
        for(line wall : walls)
        {
            t_w = (float)(wall.y2 * ray.vector_x_component - ray.y2 * ray.vector_x_component - wall.x2 * ray.vector_y_component + ray.x2 * ray.vector_y_component) /
                                (float)(wall.vector_x_component * ray.vector_y_component - wall.vector_y_component * ray.vector_x_component);
            intersect_x = (int)(wall.x2 + t_w * wall.vector_x_component);
            intersect_y = (int)(wall.y2 + t_w * wall.vector_y_component);
            
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
            removeRays.push_back(ray);
        }
        isIntersected = false;
        minDistance = 99999999.0f;
        currentDistance = 0.0f;
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