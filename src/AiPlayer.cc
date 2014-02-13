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

	HandGoodness hg = evaluateHand();

	RiskLevel risk = evaluateRisk();

	// Choose moveam
	switch (hg)
	{
	case BAD:

		//std::cout << "hg: bad";
		if (risk <= LEVEL0)
			move = CALL;
		break;
	case AVERAGE:
		//std::cout << "hg: average";
		if (risk <= LEVEL1)
			move = CALL;
		break;
	case GOOD:
		//std::cout << "hg: good";
		if (risk <= LEVEL1)
			move = RAISE;
		else if (risk <= LEVEL2)
			move = CALL;
		break;

	case EXCELLENT:
		//std::cout << "hg: excellent";
		move = RAISE;
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
	size_t activePlayers = Game::getInstance()->getActivePlayers().size();
	std::vector<int> cvv = calculateHandValue(getCards());
	std::vector<int> hvv = calculateHandValue(getHand());
	std::vector<double> oddsv = calculateOdds(cvv[0], activePlayers, hvv[0]);
	double vp = oddsv[0]; // Victory probalility

//	std::cout << "AP: " << activePlayers << " VP: " << vp << " ";

	switch (getCards().size())
	{
	case 2: // Preflop
		switch (activePlayers)
		{
		case 2:
			if (vp >= 82.5) return EXCELLENT;
			if (vp >= 60.0)	return GOOD;
			if (vp >= 45.0)	return AVERAGE;
			return BAD;
		case 3:
			if (vp >= 54.5) return EXCELLENT;
			if (vp >= 40.0)	return GOOD;
			if (vp >= 30.0)	return AVERAGE;
			return BAD;
		case 4:
			if (vp >= 40.5) return EXCELLENT;
			if (vp >= 30.0)	return GOOD;
			if (vp >= 22.5)	return AVERAGE;
			return BAD;
		case 5:
			if (vp >= 32.0) return EXCELLENT;
			if (vp >= 24.0)	return GOOD;
			if (vp >= 18.0)	return AVERAGE;
			return BAD;
		case 6:
			if (vp >= 26.0) return EXCELLENT;
			if (vp >= 20.0)	return GOOD;
			if (vp >= 15.0)	return AVERAGE;
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

AiPlayer::RiskLevel AiPlayer::evaluateRisk()
{
	Game* game = Game::getInstance();
	std::vector<Player*> players = game->getPlayers();
	size_t level = 1;
	size_t callCost = game->getCallCost();

	// callCost == 0 means call is actually check, which is risk free.
	// money == 0 means we are all in, and thus can stay in the game 
	// without any additional risk.
	if (callCost == 0 || money == 0)
	{	
//		std::cout << "RL:" << 0 << " ";
		return LEVEL0;
	}
	
	// Check how many players have raised and increase risk level accordingly.
	for (size_t i = 0; i < players.size(); i++)
	{
		if (players[i]->getLastCommand() == RAISE && players[i]->getId() != id)
			level++;
	}

	// Increase risk level if we are running out of money
	if (callCost >= money / 2)
		level++;
	if (callCost >= money)
		level++;

	// If we have raised last turn, then be more bold this turn.
	if (lastCommand == RAISE && level > 2)
		level = 2;

//	std::cout << "RL: " << level << " ";

	switch (level)
	{
	case 1:
		return LEVEL1;
	case 2:
		return LEVEL2;
	default:
		return LEVEL3;
	}
}
