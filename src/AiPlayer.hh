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
enum RiskLevel {LEVEL0, LEVEL1, LEVEL2, LEVEL3};

/*
 * Determines how good our hand is approximately.
 */
HandGoodness evaluateHand();

/*
 * Estimates how risky it is not to fold based on how much we need to pay to
 * stay in game and how many players have raised.
 */
RiskLevel evaluateRisk();

};

#endif
