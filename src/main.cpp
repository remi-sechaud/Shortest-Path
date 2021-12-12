#include <SDL.h>
#include <iostream>
#include <cmath>
#include <time.h>

#define WIN_X 200
#define WIN_Y 200

struct Point
{
    int x = rand()%WIN_X;
    int y = rand()%WIN_Y;
};

int totalDistance(short* order,int index,short n,Point* points);

void drawCircle(int radius,int pos_x,int pos_y,SDL_Renderer* renderer);

void clear(SDL_Renderer* renderer);

void displayFlip(SDL_Renderer* renderer);

void drawLines(short* order,int index,short n,Point* points,SDL_Renderer* renderer);

int main(int argc, char *argv[])
{
    int b_fit = 100000000;
    int b_bot = 0;
    srand(time(NULL));

    short n;
    int bot;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    std::cout << "n = ";
    std::cin >> n;

    std::cout << "bot = ";
    std::cin >> bot;

    std::cout << std::endl;

    struct Point points[n];

    short bots[n*bot];

    short rando;

    unsigned int fitness[bot] = {0};

    //Lancement SDL
    SDL_Init(SDL_INIT_VIDEO);

    //Création fenêtre + rendu
    window = SDL_CreateWindow("Shortest Path", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,WIN_X, WIN_Y,SDL_WINDOW_ALLOW_HIGHDPI);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    short running = 1;
    SDL_Event event;

    int points_distances[n*n];

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            points_distances[i*n+j] = pow(points[i].x-points[j].x,2) + pow(points[i].y-points[j].y,2);
        }
    }

    short temp;
    short c,d;

    for(int j = 0; j < bot; j++)
    {
        for(int i = 0; i < n; i++)
        {
            bots[j*n+i] = i;
            
        }
    }

    for(int j = 0; j < bot; j++)
    {
        for(int i = 0; i < n; i++)
        {
            c = rand()%n;
            temp = bots[j*n+c];
            bots[j*n+c] = bots[j*n+i];
            bots[j*n+i] = temp;
            
        }
    }


    while(running)
    {

        SDL_PollEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                running = 0;    
                break;

        }
        clear(renderer);
        
        for(int i = 0; i < n; i++)
        {
            drawCircle(3,points[i].x,points[i].y,renderer);
        }


        for(int i = 0; i < bot; i++)
        {    
            fitness[i] = totalDistance(bots,i*n,n,points);

            /*for(int j = 0; j < n-1;j++)
            {
                fitness[i] += points_distances[bots[i*n+j]*n+bots[i*n+j+1]];
            }*/

            if(fitness[i] < b_fit)
            {
                b_fit = fitness[i];
                b_bot = i;
            }

        }

        for(int j = 0; j < bot; j++)
        {
            if(j != b_bot)
            {
                rando = rand()%2+1;
                for(int i = 0; i < rando; i++)
                {
                    srand(time(NULL));
                    c = rand()%n;
                    d = rand()%n;
                    temp = bots[j*n+c];
                    bots[j*n+c] = bots[j*n+d];
                    bots[j*n+d] = temp;
                }
                
            }  
        }

        drawLines(bots,b_bot*n,n,points,renderer);

        displayFlip(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    

    return EXIT_SUCCESS;
}

void clear(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
}

void displayFlip(SDL_Renderer* renderer)
{
    SDL_RenderPresent(renderer);
}

void drawCircle(int radius,int pos_x,int pos_y,SDL_Renderer* renderer)
{

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    int x_min = -radius;
    int x_max = radius;
    for(int x = x_min ; x < x_max ; x++)
    {
        int y = sqrt((radius*radius)-(x*x));
        SDL_RenderDrawLine(renderer,pos_x-x,pos_y+y,pos_x-x,pos_y-y);
    }

}

void drawLines(short* order,int index,short n,Point* points,SDL_Renderer* renderer)
{
    for(int i = 0; i < n-1;i++)
    {
        SDL_RenderDrawLine(renderer,points[order[i+index]].x,points[order[i+index]].y,points[order[i+1+index]].x,points[order[i+1+index]].y);
    }
}

int totalDistance(short* order,int index,short n,Point* points)
{
    int t = 0;
    for(int i = 0; i < n-1;i++)
    {
        t += pow(points[order[i+index]].x-points[order[i+index+1]].x,2)+pow(points[order[i+index]].y-points[order[i+1+index]].y,2);
    }

    return t;

}


