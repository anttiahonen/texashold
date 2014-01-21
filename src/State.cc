#include "StateMachine.hh"
#include "GameStates.hh"
#include "Game.hh"
#include "GameUI.hh"
#include "HelperTools.hh"
#include "State.hh"

void State::executeRound()
{
	Game* game = Game::getInstance();

	std::vector<Player*> players = game->getActivePlayers();

	size_t id = game->getNextPlayerId(game->getBigBlindId()); // The player next from BBID begins the game
	size_t endId = id;
	bool first = true;

	Command lastCommand;

	while (1) // This is used to loop through valid IDs
	{
		if (id == endId && !first) // When we hit endId we've gone through one round
		{
			size_t bet = game->getCallCost();
			bool check = true;

			for (size_t i = 0; i < players.size(); i++)
			{
				if (players[i]->isActive())
					if (players[i]->getBet() != bet && players[i]->getMoney() != 0)
						check = false;
			}

			if (check)
				break;
		}

		first = false;

		lastCommand = game->getPlayerById(id)->playTurn(); // Every player plays his turn

		if (lastCommand ==  RAISE)
			endId = id;

		if (game->getActivePlayers().size() == 1)
		{
			StateMachine::getInstance()->changeState(Win::getInstance());

			return;
		}


		id = game->getNextPlayerId(id);
	}
}
