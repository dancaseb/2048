#include "gamelogic.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

//nulovani pole
void del_int(Board *board)
{
    for (int i = 0; i < board->rows * board->cols; i++)
    {
        board->board_view[i].element = 0;
        board->board_view[i].isMerged = false;
    }
}

//vytvorim strukturu Board, obsahuje pocet radku, sloupcu a pole s hodnotami
Board *board_new(int rows, int cols)
{
    Board *b = malloc(sizeof(Board));
    if (b == NULL)
    {
        perror("memory not allocated");
        exit(1);
    }
    b->cols = cols;
    b->rows = rows;
    b->board_view = malloc(rows * cols * sizeof(BoardElement));

    if (b->board_view == NULL)
    {
        perror("memory not allocated");
        exit(1);
    }
    del_int(b);
    return b;
}
//uvolneni pameti
void free_board(Board *board)
{
    free(board->board_view);
    free(board);
}


//tisk board
void print(Board *board)
{
    for (int r = 0; r < board->rows; r++)
    {
        for (int c = 0; c < board->cols; c++)
        {
            printf("%d ", board->board_view[r * board->cols + c].element);
        }
        printf("\n\n");
    }
}
//zjisti, zda se na pozici v poli nachazi nula nebo ne
bool is_occupied(Board *board, Position position)
{
    if (board->board_view[position.r * board->cols + position.c].element != 0)
    {
        return true;
    }
    return false;
}
//vygeneruje nahodne misto v poli, na kterem je nula
int rnd_Board_pos(Board *board)
{
    int board_size = board->rows * board->cols;
    //do array temp si budu ukladat pozici, na kterych je nula.
    int *array_temp = (int *)malloc(board_size * sizeof(int));
    if (array_temp == NULL)
    {
        perror("array temp not allocated");
        exit(1);
    }

    int array_length = 0;
    /*for (int i = 0; i < board_size; i++)
    {
        array_temp[i] = 0;
    }*/
    //nastavim hodnoty array_temp na 0
    memset(&array_temp[0], 0, board_size * sizeof(int));

    for (int r = 0; r < board->rows; r++)
    {
        for (int c = 0; c < board->cols; c++)
        {
            Position position;
            position.r = r;
            position.c = c;
            if (is_occupied(board, position))
            {
                continue;
            }
            else
            {
                array_temp[array_length] = r * board->cols + c;
                array_length++;
            }
        }
    }
   
    //ukoncovaci podminka hry, pokud jsou vsechny pozice obsazene, hrac prohral
    if (array_length == 0)
    {
        free(array_temp);
        return -1;
    }
    //vygeneruji nahodnou pozici

    int random = rand() % array_length;
    //chci vratit prvek boardu, na kterem je volna pozice
    int random_pos = array_temp[random];

    free(array_temp);

    return random_pos;
}
//generovani 2 nebo 4
int rnd_two_four()
{

    int random = rand() % 2 ? 4 : 2;


    return random;
}

void generate_rnd_nmb(Board *board)
{
    int pos = rnd_Board_pos(board);
    int nmb = rnd_two_four();

    board->board_view[pos].element = nmb;
}



//vraci krajni index pro smer vlevo nebo vpravo, nahora a dolu
int is_end(Board *b, Position direction, int row_col)
{
    if (direction.c == -1)
    {
        return (b->cols) * row_col + (b->cols) - 1;
    }
    else if (direction.c == 1)
    {
        return row_col * b->cols;
    }
    else if (direction.r == 1)
    {
        return ((b->rows - 1) * b->cols) + row_col;
    }
    else if (direction.r == -1)
    {
        return row_col;
    }
    return 0;
}

int find_zero(Board *b, Position direction, int row_col)
{
    int index_zero;
    bool last_zero_found = false;
    int start_index;
    //startovni index pro jednotlive smery
    if (direction.c == 1)
    {
        start_index = (b->cols) * row_col + (b->cols) - 1;
    }
    else if (direction.c == -1)
    {
        start_index = (b->cols) * row_col;
    }
    else if (direction.r == 1)
    {
        start_index = row_col;
    }
    else if (direction.r == -1)
    {
        start_index = ((b->rows) - 1) * (b->cols) + row_col;
    }
    //at nic nedela, kulicka se nepohybuje
    else
    {
        return -1;
    }
    while (!last_zero_found)
    {
        if (b->board_view[start_index].element == 0)
        {
            index_zero = start_index;
            last_zero_found = true;
            break;
        }
        if (start_index == is_end(b, direction, row_col))
        {
            index_zero = start_index;
            last_zero_found = true;
        }
        start_index -= direction.c - (b->cols) * direction.r;
    }
    return index_zero;
}

int find_nonzero(Board *b, Position direction, int index_zero, int row_col)
{
    bool first_nonzero_found = false;
    int first_index;

    if (index_zero == is_end(b, direction, row_col))
    {
        return index_zero;
    }
    //prvni index je jeden vedle od indexu, kde byla nalezena prvni nula v radku nebo sloupci

    first_index = index_zero + (-direction.c) + direction.r * b->cols;
    //index nesmi byt zaporny nebo vetsi nez pocet radku ci sloupcu, zalezi v jakem jdeme smeru (jedna slozka bude vzdy nulova)
    if (first_index < 0)
    { //|| first_index > row_col*is_end(b,direction,row_col)+row_col*b->cols || first_index > row_col*is_end(b,direction,row_col)+row_col)/*abs(direction.c*b->cols+b->cols*row_col + (direction.r*b->rows-1)+row_col))*/{
        return index_zero;
    }

    while (!first_nonzero_found)
    {
        if (first_index == is_end(b, direction, row_col))
        {
            first_nonzero_found = true;
            break;
        }

        if (b->board_view[first_index].element == 0)
        {
            first_index -= direction.c - (b->cols) * direction.r;
        }
        else
        {
            first_nonzero_found = true;
            break;
        }
    }
    return first_index;
}

Position move(Board *b, Position direction, int row_col)

{
    Position twoIndexMove;
    twoIndexMove.r = 0;
    twoIndexMove.c = 0;

    if (direction.r == 0 && direction.c == 0)
    {
        return twoIndexMove;
    }

    int end_index = -1;

    bool last_index_found = false;
    bool first_index_found = false;
    int current_column;

    //hledam index,
    end_index = find_zero(b, direction, row_col);

 

    int first_index = find_nonzero(b, direction, end_index, row_col);


    //pokus se first index rovna s end index, tak to znamena ze radek i sloupec jsou plne a nema cenu je zaplnovat
    if (first_index == end_index)
    {
        return twoIndexMove;
    }

    //nastavim si prvni index a posledni index, ktere se maji pohnout.
    if (b->board_view[first_index].element == 0 && b->board_view[end_index].element == 0)
    {
        return twoIndexMove;
    }
    twoIndexMove.r = first_index;
    twoIndexMove.c = end_index;
    b->board_view[end_index].element = b->board_view[first_index].element;
    b->board_view[first_index].element = 0;
   



    return twoIndexMove;

}

void previous_element(Board *b, Position direction, int previous_index, int row_col);
void merge1(Board *b, Position direction, int row_col)
{
    Position dir_temp = direction;
    dir_temp.r *= -1;
    dir_temp.c *= -1;

    //zjisteni odkud jdu
    int i = is_end(b, dir_temp, row_col);
    int previous_index = i;
    //po kam az jdu
    while (i != is_end(b, direction, row_col))
    {
        previous_index -= direction.c - (b->cols) * direction.r;

        //prechodzi prvek je stejny, dojde k merge a prechozi prvek se nastavi na nulu
        if (b->board_view[i].element == b->board_view[previous_index].element)
        {
            b->board_view[i].element = 2 * b->board_view[previous_index].element;
            b->board_view[previous_index].element = 0;
            //zbyetek prvku se posunu o jednu pozici
            previous_element(b, direction, previous_index, row_col);
        }
        //posun i, bud kladne nebo zaporne, zalezi na smeru

        i -= direction.c - (b->cols) * direction.r;
    }
}

void previous_element(Board *b, Position direction, int index, int row_col)
{
    //ziska prechozi prvek
    int previous_index = index - direction.c + (b->cols) * direction.r;
    // muze se stat, ze previous index bude -1, pote nehcceme pokracovat v cyklu, taky zalezi
    //na smeru, proto dve podminky. Bud index prekrocim nebo bude index mensi.
    if (previous_index < is_end(b, direction, row_col) && (direction.r == -1 || direction.c == 1))
    {
        return;
    }
    if (previous_index > is_end(b, direction, row_col) && (direction.r == 1 || direction.c == -1))
    {
        return;
    }

    b->board_view[index].element = b->board_view[previous_index].element;
    b->board_view[previous_index].element = 0;
    //ukoncovaci podminka cyklu, pokud se dostaneme na posledni mozny index, nechceme pokracovat
    if (previous_index == is_end(b, direction, row_col))
    {

        return;
    }

    previous_element(b, direction, previous_index, row_col);
}

Position mergeWhat(Board *b, Position direction, int row, int col)
{
    int i;
    Position twoPointsToChange;
    twoPointsToChange.r = 0;
    twoPointsToChange.c = 0;
    Position dir_temp = direction;
    dir_temp.r *= -1;
    dir_temp.c *= -1;
    if (direction.c == -1)
    {
        i = col + b->cols * row;
    }
    else if (direction.c == 1)
    {
        i = b->cols - 1 - col + b->cols * row;
    }
    else if (direction.r == 1)
    {
        i = row * b->cols + col;
    }
    else if (direction.r == -1)
    {
        i = (b->rows - 1 - row) * b->cols + col;
    }

    //int index_1 = is_end(b,dir_temp,row_col);
    //int index_2 = is_end(b,direction,row_col);
    //zjisteni odkud jdu

    int previous_index = i - direction.c + (b->cols) * direction.r;
    if (b->board_view[i].element == b->board_view[previous_index].element && b->board_view[i].element != 0 &&
        (b->board_view[i].isMerged == false && b->board_view[previous_index].isMerged == false))
    {
   
        twoPointsToChange.c = i;
        twoPointsToChange.r = previous_index;
    }
    return twoPointsToChange;
    //po kam az jdu
}
void merge(Board *b, int i, int previous_index)
{
    b->board_view[i].element = 2 * b->board_view[previous_index].element;
    b->board_view[previous_index].element = 0;
    b->board_view[i].isMerged = true;
}

void clear_isMerged(Board *b)
{
    for (int i = 0; i < b->rows * b->cols; i++)
    {
        b->board_view[i].isMerged = false;
    }
}
