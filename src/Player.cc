#include "Player.hh"
#include "Game.hh"
#include "GameUI.hh"
#include <sstream>
#include <stdexcept>

/*
 * Return a vector containing cards in hand and cards on the table.
 */
std::vector<Card*> Player::getCards() const
{
	Game* game = Game::getInstance();
	std::vector<Card*>table = game->getTable();
	std::vector<Card*> cards;
	for (size_t i = 0; i < hand.size(); i++)
		cards.push_back(hand[i]);
	for (size_t i = 0; i < table.size(); i++)
		cards.push_back(table[i]);
	return cards;
}

void Player::setBet(size_t newbet)
{
	if (newbet > bet)
		callWasCheck = false;
	else
		callWasCheck = true;

	if (newbet == 0)
	{
		bet = 0;
		return;
	}

	if (newbet < bet && newbet != 0)
		throw std::logic_error( "newbet < bet" );
	size_t delta = newbet - bet;
	if (delta > money)
		delta = money;
	money -= delta;
	bet += delta;
}

/*
 * Raise / bet
 */
void Player::raise()
{
	Game* game = Game::getInstance();

	if (money <= game->getCallCost())
	{
		call();
		return;
	}

	setBet(game->getRaiseCost());
	game->setHighestRaise(getBet());
	lastCommand = RAISE;
	GameUI::getInstance()->printAction(this);
//	printAction(RAISE);
}

/*
 * Call / check
 */
void Player::call()
{
	Game* game = Game::getInstance();
	setBet(game->getCallCost());
	if (callWasCheck)
		lastCommand = CHECK;
	else
		lastCommand = CALL;
	GameUI::getInstance()->printAction(this);
//	printAction(CALL);
}

/*
 * Fold
 */
void Player::fold()
{
	lastCommand = FOLD;
	folded = true;
	GameUI::getInstance()->printAction(this);
//	printAction(FOLD);
}

std::string Player::getName()
{
	std::stringstream ss;
	ss << "Player " << id;
	std::string str;
	std::getline(ss, str);
	return str;
}

/*
void Player::printAction(Command com)
{
	std::stringstream ss;

	if (id == 0)
		ss << "Human P";
	else
		ss << "Player " << id;

	switch (com)
	{
	case RAISE:
		ss << " raises";
		break;
	case CALL:
		if (money == 0)
			ss << " is all in";
		else
			if (callWasCheck)
				ss << " checks";
			else
				ss << " calls";
		break;
	case FOLD:
		ss << " folds";
		break;
	case NONE:
		ss << " has folded";
	}	

	ss << " ($" << bet << "/$" << money << ")";

	std::string str;
	std::getline(ss, str);
	GameUI::getInstance()->print(str);

	// Pause for second
	sleep(2);
}*/

