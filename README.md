This program has been tested and compiled on Ubuntu 13.04 LTS. It should
be compatible with Linux based systems. To compile this program all you need is
the g++ compiler. We did not use any external libraries. Instructions:

1. Run g++ -std=c++0x -o holdem3 *.cc
2. Run the program by typing ./holdem3

Our program is a Texas Hold'em simulator. It features a text-based user
interface. Valid inputs are HELP, PLAY, QUIT, ODDS, RAISE, CALL, FOLD (type HELP
in game for more information). It is possible to play 1vs1 (its possible to choose more players, but not stable)
heads up matches. You can play against the AI yourself, or get a 1vs1 bots only heads up match.
