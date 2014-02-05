#include <stdexcept>
#include "AiPlayer.hh"
#include "HelperTools.hh"
#include "Game.hh"
#include "GameUI.hh"
#include <iostream>

Command AiPlayer::playTurn()
{
	if (folded)
	{
		lastCommand = NONE;
		GameUI::getInstance()->printAction(this);
		return NONE;
	}

	Command move = FOLD;
	int ranValue = rand() % 100;

	HandGoodness hg = evaluateHand();

	// Choose move
	
	Game* game = Game::getInstance();
	size_t callCost = game->getCallCost();
	size_t pot = game->getPot();
	std::vector<int> cvv = calculateHandValue(getCards());
	std::vector<int> hvv = calculateHandValue(getHand());
	size_t activePlayers = game->getActivePlayers().size();
	std::vector<double> oddsv = calculateOdds(cvv[0], activePlayers, hvv[0]);
	double vp = oddsv[0]; // Victory probalility
	double potOdds = callCost / (pot + callCost);
	double RoR = (vp/100) / potOdds; //Rate of Return

	std::cout << "VP: " << vp/100 << "RoR: " << RoR << "ranValue: " << ranValue << " " << " potOdds: " << potOdds << " " << "HG: " << hg << " ";

	switch (hg)
	{
	case BAD:
		if (ranValue < 95) {
			move = FOLD;
		}
		else {
			move = RAISE; //BLUFF
		}
		//std::cout << "hg: bad";
		break;
	case AVERAGE:
		//std::cout << "hg: average";
		if (ranValue < 80) {
			move = FOLD;
		}
		if (ranValue < 85) {
			move = CALL;
		}
		else {
			move = RAISE;
		}
		break;
	case GOOD:
		//std::cout << "hg: good";
		if (ranValue < 60) {
			move = CALL;
		}
		else {
			move = RAISE;
		}
		break;

	case EXCELLENT:
		if (ranValue < 30) {
			move = CALL;
		}
		else {
			move = RAISE;
		}
		break;
	case C2C_IS_0:
		move = CALL;
		break;
	default:
		// execution should never reach this point.
		throw std::logic_error( "The AI code is broken..." );
	}

//	std::cout << " " << hand[0]->toString() << " " << hand[1]->toString() << std::endl;

	// check if everyone else is all in
	std::vector<Player*> players = Game::getInstance()->getActivePlayers();
	bool everyoneAllIn = true;
	for (size_t i = 0; i < players.size(); i++)
	{
		if (players[i]->isAllIn() == false && players[i]->getId() != id)
		{
			everyoneAllIn = false;
			break;
		}
	}

	if (move == RAISE && everyoneAllIn)
		move = CALL;

	if (move == RAISE && money == 0)
		move = CALL;

	// Make the move
	switch (move)
	{
	case RAISE:
		this->raise();
		break;
	case CALL:
		this->call();
		break;
	case FOLD:
		this->fold();
		break;
	default:
		throw std::logic_error( "Invalid move" );
	}

	return lastCommand;
}

// http://fi.pokernews.com/poker-tools/poker-odds-calculator.htm
AiPlayer::HandGoodness AiPlayer::evaluateHand()
{
	Game* game = Game::getInstance();
	size_t callCost = game->getCallCost();
	size_t pot = game->getPot();
	size_t activePlayers = game->getActivePlayers().size();
	std::vector<int> cvv = calculateHandValue(getCards());
	std::vector<int> hvv = calculateHandValue(getHand());
	std::vector<double> oddsv = calculateOdds(cvv[0], activePlayers, hvv[0]);
	double vp = oddsv[0]; // Victory probalility
	double potOdds = callCost / (pot + callCost);
	if (potOdds == 0) {
		return C2C_IS_0; //Cost to call is zero.
	}
	double RoR = vp / potOdds; //Rate of Return

	switch (getCards().size())
	{
	case 2: // Preflop
		switch (activePlayers)
		{
		case 2:
			if (RoR < 0.8) return BAD;
			if (RoR < 1.0)	return AVERAGE;
			if (RoR < 1.3)	return GOOD;
			if (RoR >= 1.3)	return EXCELLENT;
			return C2C_IS_0;
		case 3:
			if (RoR >= 54.5) return EXCELLENT;
			if (RoR >= 40.0)	return GOOD;
			if (RoR >= 30.0)	return AVERAGE;
			return BAD;
		case 4:
			if (RoR >= 40.5) return EXCELLENT;
			if (RoR >= 30.0)	return GOOD;
			if (RoR >= 22.5)	return AVERAGE;
			return BAD;
		case 5:
			if (RoR >= 32.0) return EXCELLENT;
			if (RoR >= 24.0)	return GOOD;
			if (RoR >= 18.0)	return AVERAGE;
			return BAD;
		case 6:
			if (RoR >= 26.0) return EXCELLENT;
			if (RoR >= 20.0)	return GOOD;
			if (RoR >= 15.0)	return AVERAGE;
			return BAD;
		}
	case 5: // Flop
		switch (activePlayers)
		{
		case 2:
			if (vp >= 84.5) return EXCELLENT;
			if (vp >= 60.0)	return GOOD;
			if (vp >= 51.0)	return AVERAGE;
			return BAD;
		case 3:
			if (vp >= 56.5) return EXCELLENT;
			if (vp >= 40.0)	return GOOD;
			if (vp >= 34.0)	return AVERAGE;
			return BAD;
		case 4:
			if (vp >= 42.5) return EXCELLENT;
			if (vp >= 30.0)	return GOOD;
			if (vp >= 25.5)	return AVERAGE;
			return BAD;
		case 5:
			if (vp >= 34.0) return EXCELLENT;
			if (vp >= 24.0)	return GOOD;
			if (vp >= 20.5)	return AVERAGE;
			return BAD;
		case 6:
			if (vp >= 28.0) return EXCELLENT;
			if (vp >= 20.0)	return GOOD;
			if (vp >= 17.0)	return AVERAGE;
			return BAD;
		}
	case 6: // Turn
		switch (activePlayers)
		{
		case 2:
			if (vp >= 84.5) return EXCELLENT;
			if (vp >= 60.0)	return GOOD;
			if (vp >= 54.0)	return AVERAGE;
			return BAD;
		case 3:
			if (vp >= 56.5) return EXCELLENT;
			if (vp >= 40.0)	return GOOD;
			if (vp >= 36.0)	return AVERAGE;
			return BAD;
		case 4:
			if (vp >= 42.5) return EXCELLENT;
			if (vp >= 30.0)	return GOOD;
			if (vp >= 27.0)	return AVERAGE;
			return BAD;
		case 5:
			if (vp >= 34.0) return EXCELLENT;
			if (vp >= 24.0)	return GOOD;
			if (vp >= 21.5)	return AVERAGE;
			return BAD;
		case 6:
			if (vp >= 28.0) return EXCELLENT;
			if (vp >= 20.0)	return GOOD;
			if (vp >= 18.0)	return AVERAGE;
			return BAD;
		}
	case 7: // River
		switch (activePlayers)
		{
		case 2:
			if (vp >= 84.5) return EXCELLENT;
			if (vp >= 60.0)	return GOOD;
			return BAD;
		case 3:
			if (vp >= 56.5) return EXCELLENT;
			if (vp >= 40.0)	return GOOD;
			return BAD;
		case 4:
			if (vp >= 42.5) return EXCELLENT;
			if (vp >= 30.0)	return GOOD;
			return BAD;
		case 5:
			if (vp >= 34.0) return EXCELLENT;
			if (vp >= 24.0)	return GOOD;
			return BAD;
		case 6:
			if (vp >= 28.0) return EXCELLENT;
			if (vp >= 20.0)	return GOOD;
			return BAD;
		}
	}
	// This is for debugging purposes only (the program should never reach this point):
	std::cout << "Active: " << activePlayers << std::endl;
	std::cout << "Hand: " << hand.size() << std::endl;
	std::cout << "Cards: " << getCards().size() << std::endl;
	throw std::logic_error( "AI's evaluateHand() method encountered a problem." );
}
