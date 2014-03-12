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
	//std:: cout << newbet << " is newbet and bet is " << bet << "\n";

	if (newbet < bet && newbet != 0)
		throw std::logic_error( "newbet < bet" );
	size_t delta = newbet - bet;
	if (delta > money)
		delta = money;
	money -= delta;
	Game* game = Game::getInstance();
	game->setPot(game->getPot() + delta);
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

	setBet(getBet() + game->getRaiseCost());
	game->setHighestRaise(200);
	lastCommand = RAISE;
	raises+=1;
	turns +=1;
	GameUI::getInstance()->printAction(this);
}

/*
 * Call / check
 */
void Player::call()
{
	Game* game = Game::getInstance();
	setBet(game->getCallCost() + getBet());
	game->setHighestRaise(0);
	if (callWasCheck)
		lastCommand = CHECK;
	else
		lastCommand = CALL;
	GameUI::getInstance()->printAction(this);
	turns+=1;
}

/*
 * Fold
 */
void Player::fold()
{
	lastCommand = FOLD;
	folded = true;
	GameUI::getInstance()->printAction(this);
}

std::string Player::getName()
{
	std::stringstream ss;
	ss << "Player " << id;
	std::string str;
	std::getline(ss, str);
	return str;
}
