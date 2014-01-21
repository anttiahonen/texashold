#ifndef HOLDEM_HELPERTOOLS
#define HOLDEM_HELPERTOOLS

#include <utility>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Card.hh"
#include "Deck.hh"
#include "thdefs.hh"

/*
 * The Helpertools for calculating odds and handvalues. Also used for generating/reading the Odds-tables
 * with Monte-Carlo simulation.
 */

/*
 * Reads the carddag from a external file to memory.
 */
void initCardDag();

/*
 * The Monte-Carlo function which generates the desired amount of hand/win-situation simulations.
 * This function is used for separately for different amount of players.
 * This code is commented out in the final version
 */
void initOdds(int players, int times);

/*
 * Writes the MC-simulations to an external file.
 * This code is commented out in the final version.
 */
void writeOddsToFile(int playerNum);

/*
 * Reads the written odd/win-oddtables from the external file, and the given row from there
 */
odds readOddsFromFile(int rowVal, size_t playerNum);

/*
 * Calculates the value of the given cards. This is used for determinating the winner of the game.
 * Uses Moritz Hammer's 7-card DAG look-up table, from where the given cards value is extracted.
 * Also checks the equivalance-class from the table (Copyright of Caktus Kev-eqtable).
 */
std::vector<int> calculateHandValue(std::vector<Card*>);

/*
 * Calculates the odds for the given hand(given as int rowVal).
 * The odd for winning is also calculated here, it needs the amount of players and players hand cards for this.
 * Also the odds-tables are given as parameters.
 */
std::vector<double> calculateOdds(int rowVal, size_t players, int handRowVal);

/*
 * Converses a range of equivalance numbers to a single one.
 */
int eqToInt(int eq);

/*
 * Helper-function for sorting the cards
 */
bool compareCards(Card* Card1, Card* Card2);

/*
 * Helper-function for printing the cards to humanreadable form.
 */
std::string cardsToString(std::vector<Card*> cards);

#endif
