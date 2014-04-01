#include "Game.hh"
#include <iostream>
#include <string>
#include "thdefs.hh"
#include "Player.hh"


/*
 * The UI-class. Handles the input and output of the game.
 */
class GameUI
{
public:
	/*
	 * Normal singleton getInstance method.
	 */
	static GameUI* getInstance();
	~GameUI();

	/*
	 * Clear the terminal screen.
	 */
	void clearScreen();
	
	/*
	 * Print the game welcome screen.
	 */
	void printWelcome() const;

	/*
	 * Print the game start screen.
	 */
	void printStart() const;

	/*
	 * Print the bot vs bot screen
	 */
	void printBots() const;

	/*
	 * Prompt for number of bot matches
	 */
	void printBotGames() const;

	/*
	 * Print the game turn screen. It has information about the players, their money, human player cards and the table cards.
	 */
	void printTurn(std::vector<Player*> players, Player* humanPlayer, size_t pot, std::vector<Card*> cards, size_t dealer, size_t sb, size_t bb) const;

	/*
	 * Print the game ending screen.
	 */
	void printExit(size_t winner, int arr []) const;

	/*
	 * Print the game input line.
	 */
	void printInput() const;

	/*
	 * Print the different errors from input errors.
	 */
	void printInputError() const;

	/*
	 * Print the game help screen.
	 */
	void printHelp() const;

	/*
	 * Print information about the roundwinner.
	 */
	void printRoundWinner(std::vector<Player*> winners) const;

	/*
	 * Print information about the winner.
	 */
	void printWinner(size_t winner) const;

	/*
	 * Stantard print for the UI, takes std::string as parameter.
	 */
	void print(std::string) const;

	/*
	 * Overloaded print with const char*.
	 */
	void print(const char* chr) const;

	/*
	 * Function for reading and processing the input.
	 */
	void readInput();

	/*
	 * Function for getting yes or no from user.
	 */
	bool confirm(std::string prompt) const;
	/*
	 * Helper function to reset the UI-state.
	 */
	void setGameStart(size_t game);

	/*
	 * Get the ingame action from the player (RAISE,CALL,FOLD,ODDS).
	 */
	Command playerActionPrompt();

	/*
	 * Print the latest action done by the given player.
	 */
	void printAction(Player* player) const;

	/*
	 * Print the proper taunt for the proper situation, uses random goodness.
	 */
	void printTaunt(int state, Player* player) const;

	/*
	 * Print the odds for the human player.
	 */
	void printOdds(int rowVal, int players, int handRowVal) const;
	
	/*
	 * Return the latest user given input.
	 */
	Command getLatestInput() const;

	/*
	 * Return the number of players.
	 */
	int getPlayerNum() const;

	/*
	 * Return the number of bot matches to play.
	 */
	int getTimesToPlay() const;


private:
	GameUI() : input(NONE), gameStart(0), playerNum(2), timesToPlay(1)
	{ }
	static GameUI* instance;

	Command input;
	size_t gameStart;
	int playerNum;
	int timesToPlay;
};
