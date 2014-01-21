#include <stdexcept>
#include "HumanPlayer.hh"
#include "GameUI.hh"
//#include <sstream>
//#include <iostream>

Command HumanPlayer::playTurn()
{
	if (folded)
	{
		lastCommand = NONE;
		GameUI::getInstance()->printAction(this);
		return NONE;
	}

	Command com = CALL;
	bool commandOk = false;

	if (!isAllIn()) // if not all in: ask user what to do
	{
		GameUI* ui = GameUI::getInstance();

		while (!commandOk)
		{
			com = ui->playerActionPrompt();

			commandOk = true;

			// Confirm stupid actions
			if (com == FOLD && Game::getInstance()->getCallCost() == 0)
			{
				if (!ui->confirm("Are you sure?"))
					commandOk = false;
			}
			if (com == RAISE)
			{
				std::vector<Player*> active = Game::getInstance()->getActivePlayers();
				bool everyoneAllIn = true;
				for (size_t i = 0; i < active.size(); i++)
				{
					if (!active[i]->isAllIn() && active[i]->getId() != id)
					{
						everyoneAllIn = false;
						break;
					}
				}
				if (everyoneAllIn)
				{
					if (!ui->confirm("Are you sure?"))
						commandOk = false;
				}
			}
		}
	}

	switch (com)
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
		throw std::logic_error( "Invalid Command" );
	}

	return lastCommand;
}
