#ifndef GAMECHANGE_H_
#define GAMECHANGE_H_
#include "structures.h"
#include "gamelogic.h"
#include "rendering.h"


void moveTiles(Tile* tile, Tile* square, Board* board, Position dimensions, Position direction);
void simulateMoveAllTiles(Tile* tile, Tile* square,Position direction,Position dimensions, double elapsed_ms);
void mergeAll(Board *board, Tile *tile, Tile *square, Position dimensions, Position direction);
int generateNewNumber(Board *board, Tile *tile, Tile *square, Position dimensions);
#endif