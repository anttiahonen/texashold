#ifndef HOLDEM_DECK
#define HOLDEM_DECK

#include <stdlib.h>
#include <time.h>
#include <vector>
#include "Card.hh"

/**
 * Deck of cards. Implemets suffle and draw card.
 * Deck handles the memory management of all the cards in the game.
 */
class Deck
{
public:

/*
 * Create a new deck of 52 cards.
 */
Deck() 
{

	for (size_t color = 0; color < 4; color++)
	{
		for (size_t value = 2; value < 15; value++)
		{
			Card* card = new Card(color, value);
			deck.push_back(card);
			cards.push_back(card);
		}
	}
}

~Deck()
{
	for (size_t i = 0; i < cards.size(); i++)
	{
		delete cards[i];
	}
}

/*
 * Returns a card from the top of the deck, and removes it from the deck.
 */
Card* drawCard()
{
	Card* returnCard = deck[deck.size()-1];
	deck.pop_back();
	return returnCard;
}

/*
 * Shuffle the deck.
 */
void shuffle()
{
	for (size_t i = 0; i < deck.size();  i++)
	{
		size_t j = rand() % deck.size();
		swap(i, j);
	}
}

private:
void swap(size_t i, size_t j)
{
	Card* tmp = deck[i];
	deck[i] = deck[j];
	deck[j] = tmp;
}

// Cards in the deck
std::vector<Card*> deck;
// All cards in the game 
std::vector<Card*> cards;

};

#endif
