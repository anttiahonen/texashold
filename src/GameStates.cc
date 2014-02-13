#include "StateMachine.hh"
#include "GameStates.hh"
#include "Game.hh"
#include "GameUI.hh"
#include "HelperTools.hh"

#define BLIND_UPDATE_INTERVAL 10

/*
 *	Init singletons
 */

PreFlop* PreFlop::instance;
Flop* Flop::instance;
Turn* Turn::instance;
River* River::instance;
Win* Win::instance;
End* End::instance;

/*
 *	Implementation of PreFlop class
 */

int PreFlop::roundCounter = 0;

PreFlop* PreFlop::getInstance()
{
	if (!instance)
		instance = new PreFlop;

	return instance;
}

/*
 * The enter function is the initialization of the game round so to speak. For PreFlop it means the hand cards are dealt
 * and the blinds are paid.
 */
void PreFlop::enter()
{
	Game* game = Game::getInstance();
	game->setPot(0);

	roundCounter++;

	if (roundCounter % BLIND_UPDATE_INTERVAL == 0) // Blind Update Interval is adjustable
		game->raiseBlinds();

	GameUI* gameUI = GameUI::getInstance();

	std::vector<Card*> card(2); // Card vector for storing cards to be dealt

	std::vector<Player*> players = game->getPlayers();

	game->resetDeck(); // Deck is reset, that means it's reassembled and shuffled after each round

	for (size_t i = 0; i < players.size(); i++) // Loop the players
	{
		for (size_t c = 0; c < 2; c++) // Draw the cards
		{
			 card[c] = game->getDeck()->drawCard();
		}

		players[i]->resetFold(); // Reset player state
		players[i]->setHand(card[0], card[1]); // Deal cards

		if (i == 0)
		{
			gameUI->print("\nPreflop cards dealt:");
			gameUI->print(cardsToString(card));
		}
	}

	// Paying the blinds :>
	game->getPlayerById(game->getSmallBlindId())->setBet(game->getSmallBlind());

	game->getPlayerById(game->getBigBlindId())->setBet(game->getBigBlind());

}

/*
 * The execute function is the implementation of the game round. Players play their turn in order. Afterwards this the
 * function checks the number of players to determine whether to move to the next round or to the win state.
 */
void PreFlop::execute()
{
	executeRound();

	if (Game::getInstance()->getActivePlayers().size() > 1)
		StateMachine::getInstance()->changeState(Flop::getInstance());
	else
		StateMachine::getInstance()->changeState(Win::getInstance());
}

/*
 * Exit is called when all the players have played their turns. It resets the players' variables and collects the pot
 * money.
 */
void PreFlop::exit()
{
	Game* game = Game::getInstance();

	std::vector<Player*> players = game->getPlayers();

	for (size_t i = 0; i < players.size(); i++)
	{
		//game->setPot(game->getPot() + players[i]->getBet());  this is collecting the bets and adding them to the pot

		players[i]->setBet(0);
		players[i]->resetLastCommand();
	}
}

/*
 *	Implementation of Flop class
 */

Flop* Flop::getInstance()
{
	if (!instance)
		instance = new Flop;

	return instance;
}

/*
 * The enter function is the initialization of the game round so to speak. For Flop it means the three table cards are
 * dealt.
 */
void Flop::enter()
{
	Game* game = Game::getInstance();

	GameUI* gameUI = GameUI::getInstance();

	for (size_t i = 0; i < 3; i++)
		game->addToTable(game->getDeck()->drawCard());

	gameUI->print("\nFlop cards dealt:");

	gameUI->print(cardsToString(game->getTable()));
}

/*
 * The execute function is the implementation of the game round. Players play their turn in order. Afterwards this the
 * function checks the number of players to determine whether to move to the next round or to the win state.
 */
void Flop::execute()
{
	executeRound();

	if (Game::getInstance()->getActivePlayers().size() > 1)
		StateMachine::getInstance()->changeState(Turn::getInstance());
	else
		StateMachine::getInstance()->changeState(Win::getInstance());

}

/*
 * Exit is called when all the players have played their turns. It resets the players' variables and collects the pot
 * money.
 */
void Flop::exit()
{
	Game* game = Game::getInstance();

	std::vector<Player*> players = game->getPlayers();

	for (size_t i = 0; i < players.size(); i++)
	{

		players[i]->setBet(0);
		players[i]->resetLastCommand();
	}
}

/*
 *	Implementation of Turn class
 */
Turn* Turn::getInstance()
{
	if (!instance)
		instance = new Turn;

	return instance;
}

/*
 * The enter function is the initialization of the game round so to speak. For turn it means the single table card is
 * dealt.
 */
void Turn::enter()
{
	Game* game = Game::getInstance();

	GameUI* gameUI = GameUI::getInstance();

	game->addToTable(game->getDeck()->drawCard());

	gameUI->print("\nTurn card dealt:");

	gameUI->print(cardsToString(game->getTable()));
}

/*
 * The execute function is the implementation of the game round. Players play their turn in order. Afterwards this the
 * function checks the number of players to determine whether to move to the next round or to the win state.
 */
void Turn::execute()
{
	executeRound();

	if (Game::getInstance()->getActivePlayers().size() > 1)
		StateMachine::getInstance()->changeState(River::getInstance());
	else
		StateMachine::getInstance()->changeState(Win::getInstance());
}

/*
 * Exit is called when all the players have played their turns. It resets the players' variables and collects the pot
 * money.
 */
void Turn::exit()
{
	Game* game = Game::getInstance();

	std::vector<Player*> players = game->getPlayers();

	for (size_t i = 0; i < players.size(); i++)
	{
		players[i]->setBet(0);
		players[i]->resetLastCommand();
	}
}

/*
 *	Implementation of River class
 */
River* River::getInstance()
{
	if (!instance)
		instance = new River;

	return instance;
}

/*
 * The enter function is the initialization of the game round so to speak. For River it means the single, last table card
 * is dealt.
 */
void River::enter()
{
	Game* game = Game::getInstance();

	GameUI* gameUI = GameUI::getInstance();

	game->addToTable(game->getDeck()->drawCard());

	gameUI->print("\nRiver card dealt:");

	gameUI->print(cardsToString(game->getTable()));
}

/*
 * The execute function is the implementation of the game round. Players play their turn in order. Afterwards this the
 * function checks the number of players to determine whether to move to the next round or to the win state.
 */
void River::execute()
{
	executeRound();

	StateMachine::getInstance()->changeState(Win::getInstance());

}

/*
 * Exit is called when all the players have played their turns. It resets the players' variables and collects the pot
 * money.
 */
void River::exit()
{
	Game* game = Game::getInstance();

	std::vector<Player*> players = game->getPlayers();

	for (size_t i = 0; i < players.size(); i++)
	{

		players[i]->setBet(0);
		players[i]->resetLastCommand();
	}
}

/*
 *	Implementation of Win class
 */
Win* Win::getInstance()
{
	if (!instance)
		instance = new Win;

	return instance;
}

/*
 * The enter function is the initialization of the game round so to speak. For PreFlop it means the hand cards are dealt
 * and the blinds are paid.
 */
void Win::enter()
{
	
}

/*
 * The execute function is the implementation of the game round. In the Win class it is used to determine the winner of
 * the game and give him his due moneyz.
 */
void Win::execute()
{
	Game* game = Game::getInstance();

	std::vector<Player*> activePlayers = game->getActivePlayers();

	std::vector<Player*> winners;
	winners.push_back(activePlayers[0]);
	int winnerHandValue = calculateHandValue(activePlayers[0]->getCards())[1];

	if (activePlayers.size() > 1)
	{
		for (size_t i = 1; i < activePlayers.size(); i++)
		{
			if (calculateHandValue(activePlayers[i]->getCards())[1] < winnerHandValue)
			{
				winners.clear();
				winners.push_back(activePlayers[i]);
				winnerHandValue = calculateHandValue(activePlayers[i]->getCards())[1];
			}
			else if (calculateHandValue(activePlayers[i]->getCards())[1] == winnerHandValue)
			{
				winners.push_back(activePlayers[i]);
			}
		}
	}

	GameUI::getInstance()->printRoundWinner(winners);

	if (winners.size() > 1) // draw
	{
		size_t size = winners.size();

		for (size_t i = 0; i < size; i++)
			winners[i]->giveMoney(game->getPot() / size);
	}
	else
	{
		winners[0]->giveMoney(game->getPot());
	}

	std::vector<Player*> players = game->getPlayers();

	for (size_t i = 0; i < players.size(); i++)
	{
		if (players[i]->getMoney() == 0)
		{
			game->removePlayer(players[i]->getId());

			GameUI::getInstance()->printTaunt(3, players[i]);

			players = game->getPlayers();

			i = 0;
		}
	}

	game->setPot(0);

	game->updateDealer();

	if (players.size() > 1)
		StateMachine::getInstance()->changeState(PreFlop::getInstance());
	else
		StateMachine::getInstance()->changeState(End::getInstance());
}

/*
 * 
 */
void Win::exit()
{
	Game::getInstance()->clearTable();
}

/*
 *	Implementation of End class
 */
End* End::getInstance()
{
	if (!instance)
		instance = new End;

	return instance;
}

/*
 * The enter function is the initialization of the game round so to speak. For PreFlop it means the hand cards are dealt
 * and the blinds are paid.
 */
void End::enter()
{

}

/*
 * The execute function is the implementation of the game round. End state is the state the game moves to when it ends,
 * i.e. there is only one player left.
 */
void End::execute()
{
	Game* game = Game::getInstance();

	std::vector<Player*> players = game->getPlayers();

	if (players.size() == 1)
	{
		GameUI::getInstance()->printWinner(players[0]->getId());

		GameUI::getInstance()->printExit();
	}
}

/*
 * 
 */
void End::exit()
{
	
}


