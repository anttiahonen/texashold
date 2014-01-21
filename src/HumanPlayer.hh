#ifndef HOLDEM_HPLAYER
#define HOLDEM_HPLAYER

#include "Player.hh"

/*
 * Implementation of the Human Player.
 */
class HumanPlayer : public Player
{
public:

/*
 * Constructor
 * Parameters are player's ID and start money.
 */
HumanPlayer(size_t id, size_t money)
: Player(id, money)
{ }

/*
 * This method uses the GameUI class to ask the user what action to take,
 * and executes it.
 */
Command playTurn();

/*
 * Overrides the getName() method of Player class.
 */
std::string getName()
{
	return "Human Player";
}
};

#endif
