//  NQueens - main.cpp
//  by Bill Weinman <http://bw.org/>
//  Copyright (c) 2014 The BearHeart Group LLC
//

#include <cstdio>
#include <cstring>
#include "BWCLS.h"
#include "NQ.h"

#define Ndefault 8

static const char * noneswitch = "none";
static const char * shortswitch = "short";
static const char * boardswitch = "board";
static const char * longswitch = "long";
static const char * helpswitch = "help";

void help( const char * );

int main(int argc, const char * argv[])
{
    BWCLS cls(argc, argv);
    
    bool shortflag = cls[shortswitch].have_value();
    bool boardflag = cls[boardswitch].have_value() || cls[longswitch].have_value();
    bool noneflag = cls[noneswitch].have_value();
    bool helpflag = cls[helpswitch].have_value();

    if(helpflag) {
        help(cls.get_values()[0].c_str());
        return 0;
    }

    unsigned n = Ndefault;
    BWCLS::value_vector v = cls.get_values();
    if(v.size() > 1) n = atoi(cls.get_values()[1].c_str());

    if(noneflag) boardflag = shortflag = false;

    printf("Solving for %d queens:\n", n);
    NQ q(n);
    if(boardflag) q.boardflag(boardflag);
    if(shortflag) q.shortflag(shortflag);
    q.solve();
    printf("I found %ld solutions.\n", q.solution_count());
    return 0;
}

void help( const char * me )
{
    printf("usage: %s [switch] [n]\n\n", me);
    printf("n (default: 8): size of board and number of queens.\n\n");
    printf("switch (default: --none):\n");
    printf("  --none      summary only\n");
    printf("  --short     short format\n");
    printf("  --board     board format\n");
    printf("  --long      synonym for --board\n");
    printf("  --help      this message\n");
    printf("\nversion %s\n", NQ::version());
    exit(0);
}
