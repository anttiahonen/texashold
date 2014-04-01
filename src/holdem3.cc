#include "Game.hh"
#include <cstdio>
#include <iostream>
#include "GameUI.hh"
#include "HelperTools.hh"
#include "StateMachine.hh"
#include "thdefs.hh"
#include <unistd.h>

int main(void)
{

	GameUI* gameUI = GameUI::getInstance();
	gameUI->clearScreen();
	gameUI->printWelcome();

        while (gameUI->getLatestInput() != PLAY)
        {
                gameUI->printInput();
                gameUI->readInput();
                if (gameUI->getLatestInput() == HELP)
                        gameUI->printHelp();
                else if (gameUI->getLatestInput() != PLAY)
                        gameUI->printInputError();
	}
        gameUI->printStart();
        while (gameUI->getLatestInput() != NUMS)
        {
                gameUI->printInput();
                gameUI->readInput();
                if (gameUI->getLatestInput() != NUMS)
			gameUI->printInputError();
        }
	if (gameUI->getPlayerNum() == 2){
		gameUI->printBots();
		while (gameUI->getLatestInput() != BOTS && gameUI->getLatestInput() != NOBOTS)
		{
		        gameUI->printInput();
		        gameUI->readInput();
		        if (gameUI->getLatestInput() != BOTS && gameUI->getLatestInput() != NOBOTS)
				gameUI->printInputError();
		}
	}

	gameUI->clearScreen();

	if (gameUI->getLatestInput() == BOTS)
	{
		gameUI->printBotGames();
                gameUI->printInput();
                gameUI->readInput();
	}
	
	int stopper = gameUI->getTimesToPlay();

	/*std::stringstream gameFileName;
	gameFileName << "game.txt";
	std::string gameFileStr;
	gameFileName >> gameFileStr;

	std::ofstream gameFile;
	gameFile.open(gameFileStr);
	std::stringstream ss;*/

	int p1Wins = 0;
	int p2Wins = 0;

	for (int index = 0; index < stopper; index++){

		initCardDag();

		Game* game = 0;
		StateMachine* stateMachine = 0;

		if (gameUI->getLatestInput() == BOTS)
		{
			game = Game::startGame(gameUI->getPlayerNum(), 5000, 50, 100, true);
		} else {
			game = Game::startGame(gameUI->getPlayerNum(), 5000, 50, 100, false);
		}

		stateMachine = StateMachine::getInstance();

		gameUI->printTurn(game->getPlayers(), game->getPlayerById(0), game->getPot(), game->getTable(), game->getDealerId(), game->getSmallBlindId(), game->getBigBlindId());
		
		int checker = 0;
		// loop until the game has ended
		while (checker < 1)
		{
			if (game->getPlayers().size() != 1)
		        {
				stateMachine->update();
				if (game->getPlayers().size() != 1)
				{
				        //gameUI->clearScreen();
		                        gameUI->printTurn(game->getPlayers(), game->getPlayerById(0), game->getPot(), game->getTable(), game->getDealerId(), game->getSmallBlindId(), game->getBigBlindId());
		                }
		        }
		        else
		        {
		                gameUI->printTurn(game->getPlayers(), game->getPlayerById(0), game->getPot(), game->getTable(), game->getDealerId(), game->getSmallBlindId(), game->getBigBlindId());
		                gameUI->printWinner(game->getPlayers()[0]->getId());
				int arr [2] = { Win::getInstance()->handsWon[0], Win::getInstance()->handsWon[1]};
		                gameUI->printExit(game->getPlayers()[0]->getId(), arr);
				if (game->getPlayers()[0]->getId() == 0)
					p1Wins++;
				else
					p2Wins++;
				checker = 1;
		        }

		}

	}

	std::cout << "PLayer 0 wins: " << p1Wins << std::endl;
	std::cout << "PLayer 1 wins: " << p2Wins << std::endl;

}

