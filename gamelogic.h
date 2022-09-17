#ifndef GAMELOGIC_H_
#define GAMELOGIC_H_
#include <stdbool.h>
#include "structures.h"
/*
typedef struct{ 
    int* board_view;  
    int rows;
    int cols;
    
}Board;

typedef struct{
    int r;
    int c;

}Position;*/


Board* board_new(int rows, int cols);
void free_board(Board* board);
void del_int(Board* board);
void print(Board* board);
void clear_isMerged(Board *b);

bool is_occupied(Board* board, Position position);
int rnd_Board_pos(Board* board);
int rnd_two_four();
void generate_rnd_nmb(Board* board);

int is_end(Board*b, Position direction, int row_col);
int find_zero(Board* b, Position direction, int row_col);
int find_nonzero(Board* b, Position direction, int index_zero, int row_col);

Position move(Board* b, Position direction, int row_col);
void previous_element(Board* b, Position direction, int previous_index, int row_col);

Position mergeWhat(Board* b, Position direction, int row, int col);
void merge(Board* b, int i, int previous_index);





#endif