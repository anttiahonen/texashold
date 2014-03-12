#include <stdexcept>
#include "AiPlayer.hh"
#include "Ofstream.hh"
#include "HelperTools.hh"
#include "Game.hh"
#include "GameUI.hh"
#include <iostream>
								
Command AiPlayer::playTurn() {
	if (folded)	{
		lastCommand = NONE;
		GameUI::getInstance()->printAction(this);
		return NONE;
	}
	Game* game = Game::getInstance();
	double callCost = game->getCallCost();
	Command move = FOLD;
	int ranValue = rand() % 100;
	HandGoodness hg;
	//This is for bot vs. bot configuration.
	//it selects AI 'evaluateHand1 for firstplayer and evaluatehand2 for second player
								
	if (this->getId() == 0) {
		hg = evaluateHand1();
	}
	else {
		hg = evaluateHand2();
	}
	switch (hg) {
		case BAD:
			if (ranValue < 95) {
				if (callCost == 0) {
					move = CALL;
				}	
				else {
					move = FOLD;
				}
			}
			else {
				move = RAISE; //BLUFF
			}
			//std::cout << "hg: bad";
			break;
		case AVERAGE:
			//std::cout << "hg: average";
			if (ranValue < 80) {
				if (callCost == 0) {
					move = CALL;
				}	
				else {
					move = FOLD;
				}
			}
			if (ranValue < 87) {
				move = CALL; //Small risk, but variance in decicions reduces the possibility of opponent deducing your cards.
			}
			else {
				move = RAISE; //Bluff
			}
			break;
		case GOOD:
			if (ranValue < 75) {
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
		default:
			// execution should never reach this point.
			throw std::logic_error( "The AI code is broken..." );
	}
	//	std::cout << " " << hand[0]->toString() << " " << hand[1]->toString() << std::endl;
	// check if everyone else is all in
	std::vector<Player*> players = Game::getInstance()->getActivePlayers();
	bool everyoneAllIn = true;
	for (size_t i = 0; i < players.size(); i++)	{
		if (players[i]->isAllIn() == false && players[i]->getId() != id) {
			everyoneAllIn = false;
			break;
		}
	}
	if (move == RAISE && (everyoneAllIn || money == 0)) {
		move = CALL;
	}
							
	// Make the move
	switch (move) {
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
AiPlayer::HandGoodness AiPlayer::evaluateHand1() {
								
	Game* game = Game::getInstance();
	bool bots = game->getBotsOnly();
	double callCost = game->getCallCost();
	double pot =  game->getPot();
	std::vector<int> cvv = calculateHandValue(getCards());
	std::vector<int> hvv = calculateHandValue(getHand());
	std::vector<double> oddsv = calculateOdds(cvv[0], 2, hvv[0]);
	double vp = oddsv[0]/100; // Victory probalility
	double bet = this->getBet();
	double potOdds = callCost / (pot/2	 + callCost);
						
	//Call of the cost is zero so instead of Rate of return we use handstrength to make decicions
	//on how to continue.
	if (callCost == 0 || callCost == 50) {
		if (bots == true)
			mout << "Player 1: vp: " << vp << "\n";
		else
			std::cout << "Player 1: vp: " << vp << "\n";
		if (vp < 0.4) return BAD;
		if (vp < 0.6)	return AVERAGE;
		if (vp < 0.85)	return GOOD;
		return EXCELLENT;	
	}
	double RoR = vp / potOdds; //Rate of Return
	//std:: cout << "RoR: " << RoR << " Pot: " << pot << " Potodds: " << potOdds << " WP: " << vp <<" callcost: " << callCost <<"\n";
	switch (getCards().size()) {
	//These are the values that need to be iterated.
								
		case 2: // Preflop
			if (RoR < 0.8) return BAD;
			if (RoR < 1.1)	return AVERAGE;
			if (RoR < 1.35)	return GOOD;
			return EXCELLENT;
		case 5: // Flop
			if (RoR < 1.0) return BAD;
			if (RoR < 1.25)	return AVERAGE;
			if (RoR < 1.55)	return GOOD;
			return EXCELLENT;
		case 6: // Turn			
			if (RoR < 1.2) return BAD;
			if (RoR < 1.55)	return AVERAGE;
			if (RoR < 1.95)	return GOOD;
			return EXCELLENT;
		case 7: // River
			if (RoR < 1.4) return BAD;
			if (RoR < 1.85)	return AVERAGE;
			if (RoR < 2.15)	return GOOD;
			return EXCELLENT;
	}
							
	throw std::logic_error( "AI's evaluateHand() method encountered a problem." );
}
								
// http://fi.pokernews.com/poker-tools/poker-odds-calculator.htm
AiPlayer::HandGoodness AiPlayer::evaluateHand2() {
									
	Game* game = Game::getInstance();
	bool bots = game->getBotsOnly();
	double callCost = game->getCallCost();
	double pot =  game->getPot();
	std::vector<int> cvv = calculateHandValue(getCards());
	std::vector<int> hvv = calculateHandValue(getHand());
	std::vector<double> oddsv = calculateOdds(cvv[0], 2, hvv[0]);
	double vp = oddsv[0]/100; // Victory probalility
	double bet = this->getBet();
	double potOdds = callCost / (pot/2	 + callCost);
						
	//Call of the cost is zero so instead of Rate of return we use handstrength to make decicions
	//on how to continue.
	if (callCost == 0 || callCost == 50) {
		if (bots)
			mout << "Player 1: vp: " << vp << "\n";
		else
			std::cout << "Player 1: vp: " << vp << "\n";
		if (vp < 0.4) return BAD;
		if (vp < 0.6)	return AVERAGE;
		if (vp < 0.85)	return GOOD;
		return EXCELLENT;	
	}
				
	double RoR = vp / potOdds; //Rate of Return
				
	//std:: cout << "RoR: " << RoR << " Pot: " << pot << " Potodds: " << potOdds << " WP: " << vp <<" callcost: " << callCost <<"\n";
	if (this->getMoney() <= 600 ) {
								
		switch (getCards().size()) {
		//These are the values that need to be iterated.
										
			case 2: // Preflop
				if (RoR < 1.05) return BAD;
				if (RoR < 1.4)	return AVERAGE;
				if (RoR < 2.1)	return GOOD;
				return EXCELLENT;
			case 5: // Flop
				if (RoR < 1.25) return BAD;
				if (RoR < 1.7)	return AVERAGE;
				if (RoR < 2.2)	return GOOD;
				return EXCELLENT;
			case 6: // Turn			
				if (RoR < 1.35) return BAD;
				if (RoR < 1.8)	return AVERAGE;
				if (RoR < 2.5)	return GOOD;
				return EXCELLENT;
			case 7: // River
				if (RoR < 1.55) return BAD;
				if (RoR < 2.1)	return AVERAGE;
				if (RoR < 2.8)	return GOOD;
				return EXCELLENT;
		}
	}
	if (this->getMoney() <= 1600 ) {
		switch (getCards().size()) {
		//These are the values that need to be iterated.
							
			case 2: // Preflop
				if (RoR < 1.0) return BAD;
				if (RoR < 1.3)	return AVERAGE;
				if (RoR < 1.95)	return GOOD;
				return EXCELLENT;
			case 5: // Flop
				if (RoR < 1.1) return BAD;
				if (RoR < 1.55)	return AVERAGE;
				if (RoR < 2.0)	return GOOD;
				return EXCELLENT;
			case 6: // Turn			
				if (RoR < 1.2) return BAD;
				if (RoR < 1.65)	return AVERAGE;
				if (RoR < 2.05)	return GOOD;
				return EXCELLENT;
			case 7: // River
				if (RoR < 1.4) return BAD;
				if (RoR < 2.05)	return AVERAGE;
				if (RoR < 2.45)	return GOOD;
				return EXCELLENT;
		}
								
	}
	else {
		switch (getCards().size()) {
		//These are the values that need to be iterated.
			case 2: // Preflop
				if (RoR < 0.8) return BAD;
				if (RoR < 1.1)	return AVERAGE;
				if (RoR < 1.35)	return GOOD;
				return EXCELLENT;
			case 5: // Flop
				if (RoR < 1.0) return BAD;
				if (RoR < 1.25)	return AVERAGE;
				if (RoR < 1.55)	return GOOD;
				return EXCELLENT;
			case 6: // Turn			
				if (RoR < 1.2) return BAD;
				if (RoR < 1.55)	return AVERAGE;
				if (RoR < 1.95)	return GOOD;
				return EXCELLENT;
			case 7: // River
				if (RoR < 1.4) return BAD;
				if (RoR < 1.85)	return AVERAGE;
				if (RoR < 2.15)	return GOOD;
				return EXCELLENT;
		}
	}
	throw std::logic_error( "AI's evaluateHand() method encountered a problem." );
}
