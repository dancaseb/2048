#include "rendering.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

Tile *newTile(Position dimensions)
{
    Tile *tile = malloc(sizeof(Tile) * dimensions.r * dimensions.c);
    if (tile == NULL)
    {
        perror("memory not allocated");
        exit(1);
    }

    return tile;
}
Tile *newSquare(Position dimensions, int sizeTile, int gapTile)
{
    Tile *square = malloc(sizeof(Tile) * dimensions.r * dimensions.c);
    if (square == NULL)
    {
        perror("memory not allocated");
        exit(1);
    }
    for (int r = 0; r < dimensions.r; r++)
    {
        for (int c = 0; c < dimensions.c; c++)
        {
            square[r * dimensions.c + c].pos = r * dimensions.c + c;
            square[r * dimensions.c + c].max_pos = r * dimensions.c + c;
            square[r * dimensions.c + c].h = sizeTile;
            square[r * dimensions.c + c].w = sizeTile;
            square[r * dimensions.c + c].x = c * (sizeTile + gapTile) + gapTile;
            square[r * dimensions.c + c].y = r * (sizeTile + gapTile) + gapTile;
            square[r * dimensions.c + c].isOccupied = true;
        }
    }
    return square;
}
void setTiles(Tile *tile, Tile *square, int rows, int cols)
{
    for (int i = 0; i < rows * cols; i++)
    {

        tile[i].x = square[i].x;
        tile[i].y = square[i].y;
        tile[i].w = square[i].w;
        tile[i].h = square[i].h;
        tile[i].pos = i;
        tile[i].max_pos = i;
        tile[i].value = 0;
        tile[i].isOccupied = false;
    }
}

void setMoveTiles(Tile *tile, Tile *square, Position indexToChange, Position dimensions)
{
    if (indexToChange.r == 0 && indexToChange.c == 0)
    {
        return;
    }

    tileAtPos(tile, indexToChange.r, dimensions)->max_pos = indexToChange.c;
    // printf("moving %d\n",tileAtPos(tile, indexToChange.r, dimensions)->value);
    square[indexToChange.r].isOccupied = false;
}

void simulateMoveTiles(Tile *tile, Tile *square, Position direction, int index, double elapsed_ms)
{

    if (square[tile[index].max_pos].x - tile[index].x < 1 && direction.c == 1 && tile[index].pos != tile[index].max_pos && tile[index].value != 0)
    {
        square[tile[index].max_pos].isOccupied = true;
        tile[index].pos = tile[index].max_pos;
    }
    else if (tile[index].y - square[tile[index].max_pos].y < 1 && direction.r == 1 && tile[index].pos != tile[index].max_pos && tile[index].value != 0)
    {
        square[tile[index].max_pos].isOccupied = true;
        tile[index].pos = tile[index].max_pos;
    }
    else if (tile[index].x - square[tile[index].max_pos].x < 1 && direction.c == -1 && tile[index].pos != tile[index].max_pos && tile[index].value != 0)
    {
        square[tile[index].max_pos].isOccupied = true;
        tile[index].pos = tile[index].max_pos;
    }

    else if (-tile[index].y + square[tile[index].max_pos].y < 1 && direction.r == -1 && tile[index].pos != tile[index].max_pos && tile[index].value != 0)
    {
        square[tile[index].max_pos].isOccupied = true;
        tile[index].pos = tile[index].max_pos;
    }

    if (tile[index].pos != tile[index].max_pos)
    {
        tile[index].x += 1000 * direction.c * elapsed_ms / 1000;
        tile[index].y += 1000 * (-1) * direction.r * elapsed_ms / 1000;
    }
}

Tile *tileAtPos(Tile *tile, int findPos, Position dimensions)
{
    for (int i = 0; i < dimensions.c * dimensions.r; i++)
    {
        if (tile[i].pos == findPos && tile[i].value != 0)
        {
            return &tile[i];
        }
    }
    //return NULL;
}

void resetTilePos(Tile *tile, Tile *square, Position dimensions)
{
    for (int o = 0; o < dimensions.c * dimensions.r; o++)
    {
        for (int a = o + 1; a < dimensions.c * dimensions.r; a++)
        {
            if (tile[o].pos == tile[a].pos)
            {

                if (tile[o].value == 0)
                {
                    for (int p = 0; p < dimensions.r * dimensions.c; p++)
                    {
                        if (square[p].isOccupied == false)
                        {

                            tile[o].pos = p;
                            tile[o].max_pos = p;
                            tile[o].x = square[p].x;
                            tile[o].y = square[p].y;

                            square[p].isOccupied = true;
                            break;
                        }
                    }
                }
                else if (tile[a].value == 0)
                {
                    for (int q = 0; q < dimensions.r * dimensions.c; q++)
                    {
                        if (square[q].isOccupied == false)
                        {

                            tile[a].pos = q;
                            tile[a].max_pos = q;
                            tile[a].x = square[q].x;
                            tile[a].y = square[q].y;
                            square[q].isOccupied = true;
                            break;
                        }
                    }
                }
            }
        }
    }
}

bool checkAllTilesStopped(Tile *tile, Position dimensions, Position direction)
{

    for (int i = 0; i < dimensions.c * dimensions.r; i++)
    {
        if (tile[i].pos != tile[i].max_pos)
        {
            return false;
        }
        else if (i == dimensions.c * dimensions.r - 1 && (direction.r != 0 || direction.c != 0))
        {
            //resetTilePos(tile, square, dimensions);

            return true;
        }
    }

    return false;
}

bool setMerge(Tile *tile, Tile *square, Position dimensions, Position direction)
{
    int counter = 0;
    for (int i = 0; i < dimensions.c * dimensions.r; i++)
    {

        if (tile[i].max_pos != tile[i].pos)
        {
            return false;
        }
        else
        {
            counter++;
        }
        if (counter == dimensions.c * dimensions.r - 1 && (direction.r != 0 || direction.c != 0))
        {
            //resetTilePos(tile, square, dimensions);
            resetTilePos(tile, square, dimensions);
            return true;
        }
    }
    return false;
}

