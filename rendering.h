#ifndef RENDERING_H_
#define RENDERING_H_
#include "structures.h"


Tile *newTile(Position dimensions);
Tile *newSquare(Position dimensions, int sizeTile, int gapTile);

Tile *tileAtPos(Tile *tile, int findPos, Position dimensions);

void setTiles(Tile *tile, Tile *square, int rows, int cols);
void setMoveTiles(Tile *tile, Tile *square, Position indexToChange, Position dimensions);
void simulateMoveTiles(Tile *tile, Tile *square, Position direction, int index, double elapsed_ms);

void resetTilePos(Tile *tile, Tile *Square, Position dimensions);
bool setMerge(Tile *tile, Tile *square, Position dimensions, Position direction);
bool checkAllTilesStopped(Tile *tile, Position dimensions, Position direction);


#endif