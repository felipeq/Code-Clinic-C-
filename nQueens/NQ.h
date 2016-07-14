//  NQ.h
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#ifndef __NQueens__NQ__
#define __NQueens__NQ__

#include <cstdio>
#include <cstdlib>

#define __NQueens__VERSION "1.0.7"

class NQ {
    unsigned _n = 0;
    unsigned * _row = nullptr;
    char * _dashed_line = nullptr;
    bool _boardflag = false;
    bool _shortflag = false;
    unsigned long _solution_count = 0;

    void init ( const unsigned n );
    bool is_safe( const unsigned row, const unsigned col );
    void solve_row( const unsigned row );
    void print_shorthand();
    void print_board();
public:
    static const char * version() { return __NQueens__VERSION; }
    NQ();
    NQ( const unsigned n );
    ~NQ();
    void solve();
    void boardflag(bool f) { _boardflag = f; }
    void shortflag(bool f) { _shortflag = f; }
    unsigned long solution_count() const { return _solution_count; }
};

#endif /* defined(__NQueens__NQ__) */
