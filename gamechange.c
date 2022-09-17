#include <stdio.h>
#include "structures.h"
#include "gamelogic.h"
#include "rendering.h"

void moveTiles(Tile *tile, Tile *square, Board *board, Position dimensions, Position direction)
{
    int i_max, k_max;
    if (direction.c != 0)
    {
        i_max = dimensions.r;
        k_max = dimensions.c;
    }
    else if (direction.r != 0)
    {
        i_max = dimensions.c;
        k_max = dimensions.r;
    }
    else
    {
        i_max = 0;
        k_max = 0;
    }
    for (int i = 0; i < i_max; i++)
    {
        for (int k = 0; k < k_max; k++)
        {
            Position temp = move(board, direction, i);
            setMoveTiles(tile, square, temp, dimensions);
        }
    }
}

void simulateMoveAllTiles(Tile *tile, Tile *square, Position direction, Position dimensions, double elapsed_ms)
{
    for (int i = 0; i < dimensions.r * dimensions.c; i++)
    {

        simulateMoveTiles(tile, square, direction, i, elapsed_ms);
    }
}

void mergeAll(Board *board, Tile *tile, Tile *square, Position dimensions, Position direction)
{
    int i_max, k_max;
    if (direction.c != 0)
    {
        i_max = dimensions.r;
        k_max = dimensions.c - 1;
    }
    else if (direction.r != 0)
    {
        i_max = dimensions.r - 1;
        k_max = dimensions.c;
    }
    else
    {
        i_max = 0;
        k_max = 0;
    }
    for (int i = 0; i < i_max; i++)
    {
        for (int k = 0; k < k_max; k++)
        {
            Position result = mergeWhat(board, direction, i, k);
            if (result.r == 0 && result.c == 0)
            {

                continue;
            }
            else
            {

                resetTilePos(tile, square, dimensions);

              
                tileAtPos(tile, result.c, dimensions)->value = 2 * board->board_view[result.r].element;

                resetTilePos(tile, square, dimensions);
                if (tileAtPos(tile, result.r, dimensions) == NULL)
                {
                   perror("error");
                }
                tileAtPos(tile, result.r, dimensions)->value = 0;
               
                merge(board, result.c, result.r);
                resetTilePos(tile, square, dimensions);
            }
        }
    }

    if (direction.c != 0)
    {
        i_max = dimensions.r;
        k_max = dimensions.c;
    }
    else if (direction.r != 0)
    {
        i_max = dimensions.c;
        k_max = dimensions.r;
    }
    for (int i = 0; i < i_max; i++)
    {
        for (int k = 0; k < k_max; k++)
        {
            Position temp = move(board, direction, i);
            setMoveTiles(tile, square, temp, dimensions);
        }
    }
}

int generateNewNumber(Board *board, Tile *tile, Tile *square, Position dimensions)
{
    int rnd_pos = rnd_Board_pos(board);
    if (rnd_pos == -1)
    {
        return -1;
    }
    int rnd_num = rnd_two_four();

    resetTilePos(tile, square, dimensions);
    for (int k = 0; k < dimensions.r * dimensions.c; k++)
    {
        if (tile[k].pos == rnd_pos && tile[k].value == 0)
        {
            tile[k].value = rnd_num;
            board->board_view[rnd_pos].element = rnd_num;

            //print(board);
            //printf("\n");
        }
    }
    return 0;
}