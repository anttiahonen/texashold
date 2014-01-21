#ifndef THDEFS_HH
#define THDEFS_HH
#include <vector>

enum Command { RAISE, CALL, CHECK, FOLD, NONE, PLAY, QUIT, HELP, ODDS, NUMS, EMPTY};
enum GameState { PREFLOP, FLOP, TURN, RIVER, WIN, END };
enum Suit { SPADES = 0, HEARTS = 1, DIAMONDS = 2, CLUBS = 3};

typedef std::vector< std::vector<int> > odds;

#endif
