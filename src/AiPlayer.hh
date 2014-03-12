#ifndef HOLDEM_AIPLAYER
#define HOLDEM_AIPLAYER

#include "Player.hh"

/*
 * Implementation of the AI Player.
 */
class AiPlayer : public Player
{
public:

/*
 * Constructor
 * Parameters are player's ID and start money.
 */
AiPlayer(size_t id, size_t money)
: Player(id, money)
{ }

/*
 * This method implements the AI. It decides the appropriate action and executes it.
 * Returns: the action taken.
 */
Command playTurn();

private:
enum HandGoodness {BAD, AVERAGE, GOOD, EXCELLENT};

/*
 * Determines how good our hand is approximately.
 */
HandGoodness evaluateHand();

HandGoodness evaluateHand1();


};

#endif
