#ifndef HOLDEM_PLAYER
#define HOLDEM_PLAYER

#include <vector>
#include "Card.hh"
#include "thdefs.hh"
#include <iostream>

/**
 * Abstarct class that implements methods common to Human and AI players.
 */
class Player
{
public:

/*
 * Constructor
 * Parameters are player's ID and start money.
 */
Player(size_t id, size_t money) : id(id), money(money), bet(0), folded(false), lastCommand(NONE), raises(0), turns(0)
{}

/*
 * Takes one action (raise, call or fold).
 * Returns the type of action taken.
 */
virtual Command playTurn() = 0;

/*
 * Give the two hand cards to player.
 */
void setHand(Card* card1, Card* card2)
{
	hand.clear();
	hand.push_back(card1);
	hand.push_back(card2);
}

/*
 * Return player's ID.
 */
size_t getId() const
{
	return id;
}

/*
 * Returns true if player has not folded.
 */
bool isActive() const
{
	return !folded;
}

/*
 * Returns the player's current bet.
 */
size_t getBet() const
{
	return bet;
}

/*
 * Set the player's bet.
 * New bet must be 0 or larger than player's current bet, or an exception is thrown.
 */
void setBet(size_t bet);

/*
 * Returns the amount of money the player has.
 */
size_t getMoney()
{
	return money;
}

/*
 * Return the cards in hand.
 */
std::vector<Card*> getHand() const
{
	return hand;
}

/*
 * Return the cards in hand and the cards on the table.
 */
std::vector<Card*> getCards() const;

/*
 * Get the player's last command
 */
Command getLastCommand() const
{
	return lastCommand;
}

/*
 * Resets players's last command to NONE.
 */
void resetLastCommand()
{
	lastCommand = NONE;
}

/*
 * Sets player to active.
 */
void resetFold()
{
	folded = false;
}

/*
 * Add amount to player's money.
 */
void giveMoney(size_t amount)
{
	money += amount;
}

/*
 * Is the player all in?
 */
bool isAllIn()
{
	return money == 0;
}

/*
 * Returns the name of the player.
 */
virtual std::string getName();

protected:

/*
 * Print to UI what the player is doing.
 */

/*
 * Raise / bet
 */
void raise();

/*
 * Call / check
 */
void call();

/*
 * Fold
 */
void fold();

// player variables
const size_t id;
std::vector<Card*> hand;
size_t money;
size_t bet;
bool folded;
bool callWasCheck;
Command lastCommand;
size_t raises;
size_t turns;
};

#endif
