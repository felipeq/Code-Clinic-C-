//  NQ.cpp
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#include "NQ.h"

NQ::NQ()
{
    NQ(8);
}

NQ::NQ( const unsigned n )
{
    _n = (n < 4) ? 4 : n;
    init(_n);
}

NQ::~NQ()
{
    if(_row) free(_row);
    if(_dashed_line) free (_dashed_line);
    _row = nullptr;
    _dashed_line = nullptr;
}

void NQ::init( const unsigned n )
{
    _row = (unsigned *) calloc(n, sizeof(unsigned));
}

// Check if a position is safe
bool NQ::is_safe(const unsigned row, const unsigned col)
{
    if(!_row) return false;
    // Check queens already positioned
    for (unsigned i = 0; i < row; ++i) {
        int check_col = _row[i];

        // Now check if they're in the same row or diagonals
        unsigned diff_row = row - i;
        if (check_col == col || // Same row
            check_col == col - diff_row || // Same diagonal
            check_col == col + diff_row)   // Same diagonal
            return false;
    }
    return true;
}

// recursive n-Qs solver
void NQ::solve_row( const unsigned n )
{
    if(!_row) return;
    if (n == _n) {  // have a solution
        if(_boardflag) print_board();
        else if(_shortflag) print_shorthand();
        else ++_solution_count;
    } else {
        for (unsigned i = 0; i < _n; i++) {
            if (is_safe(n, i)) {
                _row[n] = i;
                solve_row(n + 1);
            }
        }
    }
}

void NQ::print_shorthand()
{
    if(!_row) return;
    printf("Solution #%ld: ", ++_solution_count);
    for (unsigned i = 0; i < _n; i++)
        printf("%d ", _row[i] + 1); // print 1-based numbers

    printf("\n");
}

void NQ::print_board()
{
    unsigned x, y;
    if(!_row) return;
    // make a dashed line of the correct width
    
    if(!_dashed_line) {
        const unsigned width = ( 4 * _n )  + 1;
        const size_t msize = width + 1;
        _dashed_line = (char *) malloc(msize);
        for(unsigned i = 0; i < width; ++i) _dashed_line[i] = '-';
        _dashed_line[width] = '\0';
    }
    
    printf("\nSolution #%ld:\n%s\n", ++_solution_count, _dashed_line);
    for (y = 0; y < _n; ++y) {
        for (x = 0; x < _n; ++x)
            printf("| %c ", x == _row[y] ? 'Q' : ' ');
        printf("|\n%s\n", _dashed_line);
    }
}

void NQ::solve()
{
    if(!_row) return;
    solve_row(0);
}
