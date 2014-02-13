#include "GameUI.hh"
#include "HelperTools.hh"
#include <algorithm>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdexcept>
#include "StateMachine.hh"

#define RAISE_SIZE 33
#define WIN_SIZE 17
#define FOLD_SIZE 12
#define EXIT_SIZE 8

std::string raise[] = {"I dare you to make a move.",
"Sorry pal, it had to be done.",
"Ya look like you had a long day. Why don't you let me end it?",
"Go tell mama the bad man beat ya.",
"Pay attention! Let me show you how it's done.",
"I'm going to rip your throat out.",
"Lucky for me. You really suck at this.",
"What is this, amateur night?",
"Enough fooling around. Time for you to die.",
"Fighting against you is a real confidence booster.",
"I'm gonna hang your head on my trophy wall.",
"Thanks for the entertainment, rookie.",
"I'm going to smoke you like a fat cigar.",
"Once again, you lose.",
"Not even your girlfriend is this easy.",
"You need a seeing-eye dog. I hear your mother may be available.",
"Thanks for making me have a good laugh.",
"Do you have the white flag out yet?",
"I would like to smell your fear if you didn't stink so bad.",
"You are very good at losing, my friend.",
"If I knew it was this easy, I'd have left my money home.",
"Just shoot yourself and save me the trouble.",
"Nothing can save you from the pain that is coming.",
"I wish I could let you live and shoot your mother, 'cause she's really the one to blame.",
"Next time, try raising back.",
"I'd make this painless, but then where's the fun in that?",
"Not wanting to rush you, but I have other people to skin.",
"You play like a child.",
"I appreciate the warm-up. Now, send in the real opponent.",
"When you make it easy, it isn't as fun.",
"You are a nuisance, nothing more.",
"Could you at least try to defeat me?",
"Is that all you've got?"};

std::string win[] = {"gg",
"noobs",
"l2p",
"l2p nubs",
"Nicely played, cheers m8s",
"Cheers mateys",
"Woo I'm rich!",
"Suck it bitches",
"Too easy",
"Sorry pal, it had to be done.",
"Go tell mama the bad man beat ya.",
"What is this, amateur night?",
"Thanks for the entertainment, rookie.",
"Once again, you lose.",
"Thanks for making me have a good laugh.",
"When you make it easy, it isn't as fun.",
"I appreciate the warm-up. Now, send in the real opponent."};

std::string fold[] = {"Lights out!",
"This round's on me.",
"FU",
"Not with these cards, baby!",
"Lucky!",
"Goddamn hackers",
"noobs",
"I hate this game",
"This game sucks",
"I wish I got something good for once",
"Cards like these make me want to kill someone.",
"I always lose..."};

std::string exitTaunts[] = {"Lights out!",
"FU",
"Lucky!",
"Goddamn hackers",
"noobs",
"I hate this game",
"This game sucks",
"I always lose..."};

GameUI* GameUI::instance;

GameUI* GameUI::getInstance()
{
	if (!instance)
		instance = new GameUI();
	return instance;
}

GameUI::~GameUI()
{ }

void GameUI::clearScreen()
{
	system("clear");
}

void GameUI::printWelcome() const
{
	std::cout << "\033[0;31m******************************" << std::endl;
	std::cout << "*   \033[0;33mTEXAS HOLD'EM GROUP 03   \033[0;31m*" << std::endl;
	std::cout << "******************************\033[0m" << std::endl;
	std::cout << "Welcome to the Texas Hold'Em game.\nType \"HELP\" for commands or just start playing by writing \"PLAY\"!" << std::endl; 
}

void GameUI::printStart() const
{
	std::string start = "Hi awesome HUMAN player. How many players do you want the game to have (2-6)?\nPlease type 2,3,..,6";
	std::cout << start << std::endl;
}

void GameUI::printTurn(std::vector<Player*> players, Player* humanPlayer, size_t pot, std::vector<Card*> cards, size_t dealer, size_t sb, size_t bb) const
{ 
	std::cout << std::endl;

	std::cout << "Round #" << PreFlop::getInstance()->roundCounter << "\tBlinds: ($"
		<< Game::getInstance()->getSmallBlind() << " / $" << Game::getInstance()->getBigBlind()
		<< ")" << std::endl;

	// print the players
	for (size_t i = 0; i < players.size(); i++)
	{
		if (players[i]->getId() == 0)
			std::cout << "Human P" << "\t\t";
		else
			std::cout << "Player " << players[i]->getId() << "\t";
		if (i == players.size()-1)
			std::cout << "pot\n";
	}
	// print the money
	for (size_t i = 0; i < players.size(); i++)
	{
		std::cout << players[i]->getMoney() << "$\t\t";
		if (i == players.size()-1)
			std::cout << pot << "$\n";
	}
	// print the dealer and the blinds
	for (size_t i = 0; i < players.size(); i++)
	{
		if (players[i]->getId() == dealer)
			std::cout << "DEALER" << "\t\t";
		else if (players[i]->getId() == sb && players.size() != 2)
			std::cout << "SB" << "\t\t";
		else if (players[i]->getId() == bb)
			std::cout << "BB" << "\t\t";
		else
			std::cout << "\t\t";
		if (i == players.size()-1)
			std::cout << std::endl << std::endl;
	}
	// print the player cards
	if (humanPlayer != NULL)
	{
		std::cout << "Your cards:" << std::endl;
		std::vector<Card*> humanCards = humanPlayer->getHand();
	
		for (size_t i = 0; i < humanCards.size(); i++)
		{
			std::cout << humanCards[i]->toString();
			if (i != humanCards.size()-1)
				std::cout << "  ";
			else
			std::cout << std::endl;
		}
	}
	// print the table cards

	if (cards.size() > 0)
	{
		std::cout << "Table cards:" << std::endl;
		for (size_t i = 0; i < cards.size(); i++)
		{
			std::cout << cards[i]->toString();
			if (i != cards.size()-1)
				std::cout << "  ";
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;

	sleep(2);
}

void GameUI::printExit() const
{
	std::string exit = "The game has ended. Hope you enjoyed this superawesome Texas Hold'Em group03 game";
	std::cout << exit << std::endl;
}

void GameUI::printInput() const
{
	std::cout << "Input: ";
}

void GameUI::print(std::string str) const
{
	std::cout << str << std::endl;
}

void GameUI::print(const char* chr) const
{
	std::cout << chr << std::endl;
}

void GameUI::printInputError() const
{
	if (gameStart == 0)
		std::cout << "So do you want to start playing, type \"PLAY\" or commands with \"HELP\"" << std::endl;
	else if (gameStart == 1)
		std::cout << "Please type 2,3,..,6" << std::endl;
	else if (gameStart > 1)
		std::cout << "Please type a valid command, get commands with \"HELP\"" << std::endl;
}

void GameUI::printHelp() const
{
	std::string help = "The general commands are: PLAY, QUIT, and HELP. Game commands are: \033[0;33mRAISE\033[0m, \033[0;32mCALL\033[0m, \033[0;31mFOLD\033[0m and ODDS.\nGame commands work only while playing.";
	std::cout << help << std::endl;
}

void GameUI::printRoundWinner(std::vector<Player*> winners) const
{
	std::vector<Player*> active = Game::getInstance()->getActivePlayers();
	bool showHands = active.size() > 1;

	if (showHands)
	{
		for (size_t i = 0; i < active.size(); i++)
		{
			Player* p = active[i];
			std::vector<Card*> hand = p->getHand();
			std::vector<Card*> cards = p->getCards();
			int eq = calculateHandValue(cards)[1];
			eq = eqToInt(eq);
			std::string outcome;
			switch(eq)
			{
			case 9:
				outcome = "Straight-Flush";
				break;
			case 8:
				outcome = "Four of a kind";
				break;
			case 7:
				outcome = "Full house";
				break;
			case 6:
				outcome = "Flush";
				break;
			case 5:
				outcome = "Straight";
				break;
			case 4:
				outcome = "Three of a kind";
				break;
			case 3:
				outcome = "Two pairs";
				break;
			case 2:
				outcome = "One pair";
				break;
			case 1:
				outcome = "High card";
				break;

			}
			std::cout << p->getName() << " has " << hand[0]->toString() << " " << hand[1]->toString() << " - " << outcome << std::endl;
		}
	}

	std::cout << "\033[0;33m" << std::endl;

	if (winners.size() > 1)
	{
		std::cout << "Winners of this round are: ";
		for (size_t i = 0; i < winners.size(); i++)
		{
			std::cout << winners[i]->getName();
			if (i == winners.size() - 1)
				std::cout << "!";
			else if (i == winners.size() - 2)
				std::cout << " and ";
			else
				std::cout << ", ";
		}
	}
	else
	{
		std::cout << winners[0]->getName() << " wins this round!";
	}
	std::cout << "\033[0m" << std::endl << std::endl;

	for (size_t i = 0; i < winners.size(); i++)
		printTaunt(1, winners[i]);

	sleep(2);
}

void GameUI::printWinner(size_t winner) const
{
	if (winner == 0)
		std::cout << "And the winner is..... YOU, almighty HUMAN PLAYER, congratulations! ";
	else
	{
		std::cout << "And the winner is..... PLAYER " << winner <<", congratulations!\n";

		printTaunt(1, Game::getInstance()->getPlayerById(winner));

		std::cout << std::endl;
		std::cout << std::endl;
	}
}

void GameUI::readInput()
{
	std::string kInput;
	std::cin >> kInput;
	std::transform(kInput.begin(), kInput.end(), kInput.begin(), ::toupper);

	input = NONE;
	// command for the amount of players
	if (gameStart == 1)
	{
	if (kInput.compare("2") == 0 || kInput.compare("3") == 0 || kInput.compare("4") == 0
	|| kInput.compare("5") == 0|| kInput.compare("6") == 0)
	{
		std::stringstream ss(kInput);
		ss >> playerNum;
		gameStart++;
		input = NUMS;
	}
	}

	if (kInput.compare("PLAY") == 0)
	{
		input = PLAY;
		gameStart++;
	}

	if (kInput.compare("HELP") == 0)
	{
		input = HELP;
	}

	if (kInput.compare("QUIT") == 0)
	{
		std::cout << "Are you sure you want to quit (Y/N)?" << std::endl;
		bool check = false;
		while (!check)
		{
		std::cin >> kInput;
		std::transform(kInput.begin(), kInput.end(), kInput.begin(), ::toupper);

			if (kInput.compare("Y") == 0 || kInput.compare("YES") == 0)
			{
				std::cout << "BYE BYE!!";
				delete Game::getInstance();
				delete GameUI::getInstance();
				delete StateMachine::getInstance();
				
				exit(0);
			}
			if (kInput.compare("N") == 0 || kInput.compare("NO") == 0)
			{
				std::cout << "Lets keep playing then" << std::endl;
				check = true;
				input = EMPTY;
			}
			else if (check != true)
				std::cout << "Please type Y/N" << std::endl;
		}
	}

	if (kInput.compare("CALL") == 0)
	{
		input = CALL;
	}

	if (kInput.compare("RAISE") == 0)
	{
		input = RAISE;
	}

	if (kInput.compare("FOLD") == 0)
	{
		input = FOLD;
	}

	if (kInput.compare("ODDS") == 0)
	{
		input = ODDS;
	}

}

bool GameUI::confirm(std::string prompt) const
{
	std::cout << prompt << " (Y/N)" << std::endl;

	while (true)
	{
		std::string kInput;
		std::cin >> kInput;
		std::transform(kInput.begin(), kInput.end(), kInput.begin(), ::toupper);

                if (kInput.compare("Y") == 0 || kInput.compare("YES") == 0)
			return true;

		if (kInput.compare("N") == 0 || kInput.compare("NO") == 0)
			return false;

		std::cout << "Please type Y/N" << std::endl;
	}

}

void GameUI::setGameStart(size_t game)
{
	gameStart = game;
}

Command GameUI::playerActionPrompt()
{
	Game* game = Game::getInstance();
	Player* player = game->getPlayerById(0);
	std::cout << "\nYour status\n***********\n";
	std::cout << "Money: $" << player->getMoney() << "\tBet: $" << player->getBet() << std::endl;

	size_t raiseCost = game->getRaiseCost();
	size_t callCost = game->getCallCost();

	Command com;

	bool ask = true;
	while (ask)
	{
		std::cout << "Please enter command (\033[0;33mRAISE\033[0m($" << raiseCost << "), \033[0;32mCALL\033[0m($" << callCost << "), \033[0;31mFOLD\033[0m or ODDS)!"<< std::endl;
		printInput();
		readInput();
		com = getLatestInput();
		if (com == EMPTY)
			continue;
		else if (com == ODDS)
		{
			std::vector<int> cards = calculateHandValue(player->getCards());
			std::vector<int> hand = calculateHandValue(player->getHand());
			printOdds(cards[0], game->getActivePlayers().size(), hand[0]);
		}
		else if (com != RAISE && com != CALL && com != FOLD)
			print("Invalid command!");
		else
			ask = false;
	}
	
	return com;
}

void GameUI::printAction(Player* player) const
{
	std::stringstream ss;

	if (player->getId() == 0)
		ss << "Human P";
	else
		ss << "Player " << player->getId();

	if (player->getMoney() == 0)
	{
		ss << "\033[0;32m";
		ss << " is all in";
	}
	else
	{
		switch (player->getLastCommand())
		{
		case RAISE:
			ss << "\033[0;33m";
			ss << " raises";
			break;
		case CALL:
			ss << "\033[0;32m";
			ss << " calls";
			break;
		case CHECK:
			ss << "\033[0;32m";
			ss << " checks";
			break;
		case FOLD:
			ss << "\033[0;31m";
			ss << " folds";
			break;
		case NONE:
			ss << "\033[0;31m";
			ss << " has folded";
			break;
		default:
			throw std::logic_error( "player->getLastCommand() returned something weird." );
		}
	}
	ss << "\033[0m";

	ss << " ($" << player->getBet() << "/$" << player->getMoney() << ")";

	std::string str;
	std::getline(ss, str);
	print(str);

	if (player->getLastCommand() == RAISE)
		printTaunt(0, player);
	else if (player->getLastCommand() == FOLD)
		printTaunt(2, player);

	// Pause for second
	sleep(2);
}

void GameUI::printTaunt(int state, Player* player) const
{
	if (player->getId() != 0) // taunts
	{
		switch (state)
		{
		case 0: // RAISE
			if (rand() % 4 < 1)
				return;

			std::cout << player->getName() << ": \"" << raise[ rand() % RAISE_SIZE ] << "\"" << std::endl;
			break;
		case 1: // WIN
			std::cout << player->getName() << ": \"" << win[ rand() % WIN_SIZE ] << "\"" << std::endl;
			break;
		case 2: // FOLD
			if (rand() % 2 == 1)
				return;
			std::cout << player->getName() << ": \"" << fold[ rand() % FOLD_SIZE ] << "\"" << std::endl;
			break;
		case 3: // EXIT
			std::cout << player->getName() << " has run out of money and leaves the game! ("
				<< Game::getInstance()->getPlayers().size() << " players remaining)\n\n";

			std::cout << player->getName() << ": \"" << exitTaunts[ rand() % EXIT_SIZE ] << "\"" << std::endl;
			break;
		}
	}
}

void GameUI::printOdds(int rowVal, int players, int handRowVal) const
{
	std::vector<double> playerOdds = calculateOdds(rowVal, players, handRowVal);
	std::cout << "The odds:" << std::endl;
	std::cout << "*****************************************" << std::endl;
	std::cout << "\tWin:\t\t " << (playerOdds[0] - fmod(playerOdds[0], 0.01)) << "%" << std::endl;
	std::cout << "\tStraight-Flush:  " << (playerOdds[1] - fmod(playerOdds[1], 0.01)) << "%" << std::endl;
	std::cout << "\tFour of a kind:  " << (playerOdds[2] - fmod(playerOdds[2], 0.01)) << "%" << std::endl;
	std::cout << "\tFull House:\t " << (playerOdds[3] - fmod(playerOdds[3], 0.01)) << "%" << std::endl;
	std::cout << "\tFlush:\t\t " << (playerOdds[4] - fmod(playerOdds[4], 0.01)) << "%" << std::endl;
	std::cout << "\tStraight:\t " << (playerOdds[5] - fmod(playerOdds[5], 0.01)) << "%" << std::endl;
	std::cout << "\tThree of a kind: " << (playerOdds[6] - fmod(playerOdds[6], 0.01)) << "%" << std::endl;
	std::cout << "\tTwo pairs:\t " << (playerOdds[7] - fmod(playerOdds[7], 0.01)) << "%" << std::endl;
	std::cout << "\tOne pair:\t " << (playerOdds[8] - fmod(playerOdds[8], 0.01)) << "%" << std::endl;
	std::cout << "\tHigh card:\t " << (playerOdds[9] - fmod(playerOdds[9], 0.01)) << "%" << std::endl;
	std::cout << "*****************************************" << std::endl;

}

Command GameUI::getLatestInput() const
{
	return input;
}

int GameUI::getPlayerNum() const
{
	return playerNum;
}

