
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "structures.h"
#include "gamelogic.h"
//#include "rendering.h"
#include "gamechange.h"

int main(int argc, char **argv)

{
    bool generate = true;
    bool eligibleMerge = false;
    Position dimensions;

    const int sizeTile = 90;
    const int gapTile = 10;
  
    srand(time(NULL));

    if (argv[1] == NULL)
    {
        perror("no arguments,type dimensions in format 'number'x'number'");
        return 0;
    }
    int rows, cols;
    if (sscanf(argv[1], "%dx%d", &rows, &cols) != 2)
    {
        perror("error,type dimensions in format 'number'x'number'");
        return 0;
    }

    if (rows * cols > 100)
    {
        perror("board too big");
        return 0;
    }
    if (rows < 3 || cols < 3)
    {

        perror("board too small");
        return 0;
    }
    dimensions.r = rows;
    dimensions.c = cols;
    int width = cols * sizeTile + (cols + 1) * gapTile;
    int height = rows * sizeTile + (rows + 1) * gapTile;

    // inicializace SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        exit(1);
    }

    // vytvoreni okna
    SDL_Window *window = SDL_CreateWindow(
        "2048",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        exit(1);
    }
    // vytvoreni (akcelerovaneho) rendereru
    // VSYNC - synchronizace s frekvenci monitoru
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        exit(1);
    }

    // zapnuti pruhlednosti
    if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) != 0)
    {
        fprintf(stderr, "SDL_SetRenderDrawBlendMode Error: %s\n", SDL_GetError());
        exit(1);
    }
    bool quit = false;
    Uint64 prevCounter = SDL_GetPerformanceCounter();

    SDL_Texture *tilesImages = IMG_LoadTexture(renderer, "resources/drawTile.png");
    if (tilesImages == NULL)
    {
        fprintf(stderr, "Failed to load image: %s\n", SDL_GetError());
        return 1;
    }


    Tile *square = newSquare(dimensions, sizeTile, gapTile);

    Board *board = board_new(rows, cols);

    int tilecount = rows * cols;
    Tile *tile = newTile(dimensions);
    setTiles(tile, square, rows, cols);

    //smer pohybu
    Position direction;
    direction.c = 0;
    direction.r = 0;

   
    SDL_Rect dst_square[tilecount];
    for (int i = 0; i < tilecount; i++)
    {
        dst_square[i].x = square[i].x;
        dst_square[i].y = square[i].y;
        dst_square[i].w = square[i].w;
        dst_square[i].h = square[i].h;
    }
    SDL_Rect oneTile;
    oneTile.h = 99;
    oneTile.w = 99;

    while (!quit)
    {
        // na zacatku kazdeho snimku si spocitame uplynuly cas od minuleho snimku v ms (promenna elapsed_ms)
        // vsechny simulace pak musi pracovat s timto casem (tj. urazena vzdalenost = rychlost_px_za_sekundu * elapsed_ms / 1000)
        Uint64 now = SDL_GetPerformanceCounter();
        double elapsed_ms = (now - prevCounter) * 1000 / (double)SDL_GetPerformanceFrequency();
        prevCounter = now;

        // obsluha vsech udalosti (zavreni okna, stisk klavesy, pohyb mysi ci joysticku)
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
                printf("GAME OVER");
                break;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                // key is pressed DOWN
                switch (e.key.keysym.sym)
                {
                //case SDLK_ESCAPE:
                case SDLK_q:
                    quit = true;
                    break;

                case SDLK_RIGHT:
                    clear_isMerged(board);
                    generate = true;
                    direction.c = 1;
                    direction.r = 0;
                    moveTiles(tile, square, board, dimensions, direction);

                    break;

                case SDLK_UP:
                    clear_isMerged(board);
                    generate = true;
                    direction.c = 0;
                    direction.r = 1;
                    moveTiles(tile, square, board, dimensions, direction);

                    break;
                case SDLK_DOWN:
                    clear_isMerged(board);
                    generate = true;
                    direction.c = 0;
                    direction.r = -1;
                    moveTiles(tile, square, board, dimensions, direction);

                    break;
                case SDLK_LEFT:
                    clear_isMerged(board);
                    generate = true;
                    direction.c = -1;
                    direction.r = 0;
                    moveTiles(tile, square, board, dimensions, direction);

                    break;
                }
            }
        }
        //simulace pohybu dlazdic
        simulateMoveAllTiles(tile, square, direction, dimensions, elapsed_ms);

        eligibleMerge = setMerge(tile, square, dimensions, direction);

        if (eligibleMerge == true)
        {
            //spojuje dlazdice
            mergeAll(board, tile, square, dimensions, direction);
            eligibleMerge = false;
        }

        if (checkAllTilesStopped(tile, dimensions, direction) == true && generate == true)
        {
            direction.r = 0;
            direction.c = 0;

            if (generateNewNumber(board, tile, square, dimensions) == -1)
            {
                printf("GAME OVER");
                quit = true;
            }

            generate = false;
        }

        SDL_Rect dst[tilecount];
        for (int i = 0; i < tilecount; i++)
        {
            dst[i].x = tile[i].x;
            dst[i].y = tile[i].y;
            dst[i].h = tile[i].h;
            dst[i].w = tile[i].w;
        }

        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 170, 165, 157, 255); // barva pozadi
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 216, 214, 209, 255);
        for (int i = 0; i < rows * cols; i++)
        {

            SDL_RenderFillRect(renderer, &dst_square[i]);
        }

        for (int i = 0; i < tilecount; i++)
        {
            int temp = tile[i].value;
            int helpTileInRow = 1;
            int helpTileinCol = 0;
            if (temp >= 16 && temp <= 128)
            {
                helpTileInRow = 4;
                helpTileinCol = 1;
            }
            else if (temp > 128)
            {
                helpTileInRow = 8;
                helpTileinCol = 2;
            }
            oneTile.x = 57 + 105 * (log2(temp) - helpTileInRow);
            oneTile.y = 49 + 132 * helpTileinCol;
            //prvni radek je odsazen jinak, proto dochazi k posunu o 9
            if (temp == 2 || temp == 4 || temp == 8)
            {
                oneTile.x -= 9;
            }

            SDL_RenderCopy(renderer, tilesImages, &oneTile, &dst[i]);

           
        }

        SDL_RenderPresent(renderer);
    }

   // printf("\n\n");

    //print(board);

    free(square);
    free(tile);
    free_board(board);

    return 0;
}
