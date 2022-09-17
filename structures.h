#ifndef STRUCTURES_H_
#define STRUCTURES_H_
#include <stdbool.h>
typedef struct{
    int element;
    bool isMerged;
}BoardElement;

typedef struct{ 
    BoardElement* board_view;  
    int rows;
    int cols;
    
}Board;


typedef struct{
    int r;
    int c;

}Position;

typedef struct
{
    float x, y;
    float h, w;
    int pos;
    int max_pos;
    int value;
    bool isOccupied;

} Tile;
#endif