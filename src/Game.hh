#ifndef HOLDEM_GAME
#define HOLDEM_GAME

#include "Deck.hh"
#include "Player.hh"
#include <iostream>

class Game
{
public:
	/*
	 * Normal singleton getInstance method.
	 * NOTE: startGame method can destroy the Game instance and create a new
	 * one, thus you should not store a pointer to the Game instance.
	 */
	static Game* getInstance();
	~Game();

	/*
	 * Starts a new game with given parameters.
	 */
	static Game* startGame(size_t numOfPlayers, size_t moneyAmount, size_t smallBlind, size_t bigBlind);

	/*
	 * Returns a player with given ID, or NULL if no such player exists.
	 */
	Player* getPlayerById(size_t id) const;

	/*
	 * Return all player IDs.
	 */
	std::vector<size_t> getPlayerIds() const;

	/*
	 * Returns the next player ID after the parameter ID.
	 */
	size_t getNextPlayerId(size_t id) const;

	/*
	 * Returns all players.
	 */
	std::vector<Player*> getPlayers() const
	{
		return players;
	}

	/*
	 * Returns players who have not folded.
	 */
	std::vector<Player*> getActivePlayers();

	/*
	 * Removes a player from game.
	 * Returns false, if player with given ID was not found.
	 */
	bool removePlayer(size_t id);

	/*
	 * ID of dealer.
	 */
	size_t getDealerId() const
	{
		return dealerId;
	}

	/*
	 * Changes dealer ID to the next player, and updates big and small blind IDs.
	 */
	void updateDealer();

	/*
	 * Return the amount of money player needs to pay to call.
	 */
	size_t getCallCost() const;

	/*
	 * Return the amount of money player needs to pay to raise.
	 */
	size_t getRaiseCost() const;

	/*
	 * Sets the highest bet anyone has made.
	 */
	void setHighestRaise(size_t raise);

	/*
	 * ID of small blind player.
	 */
	size_t getSmallBlindId() const
	{
		return smallBlindId;
	}

	/*
	 * ID of big blind player.
	 */
	size_t getBigBlindId() const
	{	
		return bigBlindId;
	}

	size_t getSmallBlind() const
	{
		//std::cout << "SB:" << smallBlind << std::endl;
		return smallBlind;
	}

	size_t getBigBlind() const
	{
		//std::cout << "BB:" << bigBlind << std::endl;
		return bigBlind;
	}

	/*
	 * Double the blind amounts.
	 */
	void raiseBlinds()
	{
		smallBlind *= 2;
		bigBlind *= 2;
	}

	size_t getNumOfPlayers() const
	{
		return players.size();
	}

	Deck* getDeck() const
	{
		return deck;
	}

	std::vector<Card*> getTable() const
	{
		return table;
	}

	void addToTable(Card* card)
	{
		table.push_back(card);
	}

	void clearTable()
	{
		table.clear();
	}

	void setPot(size_t pot)
	{
		Game::pot = pot;
		highestRaise = 0;
	}

	size_t getPot() const
	{
		return pot;
	}

	/*
	 * Delete the current deck and all cards, the create new deck.
	 */
	void resetDeck();

private:
	Game(size_t numOfPlayers =2, size_t moneyAmount =5000, size_t smallBlind =50, size_t bigBlind =100);
	static Game* instance;

	void updateBlindIds();

	// game variables
	Deck* deck;
	std::vector<Player*> players;
	std::vector<Card*> table;

	size_t smallBlind;
	size_t bigBlind;
	size_t highestRaise;

	size_t dealerId;
	size_t smallBlindId;
	size_t starterId;
	size_t bigBlindId;

	size_t pot;
};

#endif
