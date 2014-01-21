#include <sstream>
#include "Card.hh"
#include "thdefs.hh"

std::string Card::toString() const
{
	std::stringstream ss (std::stringstream::in | std::stringstream::out);


	switch (color)
	{
	case SPADES:
		ss << "\033[0;34m";
		ss << "♠";
		break;
	case HEARTS:
		ss << "\033[0;31m";
		ss << "♥";
		break;
	case DIAMONDS:
		ss << "\033[0;31m";
		ss << "♦";
		break;
	case CLUBS:
		ss << "\033[0;34m";
		ss << "♣";
		break;
	}
	
	switch (value)
	{
	case 10:
		ss << "T";
		break;
	case 11:
		ss << "J";
		break;
	case 12:
		ss << "Q";
		break;
	case 13:
		ss << "K";
		break;
	case 14:
		ss << "A";
		break;
	default:
		ss << value;
		break;
	}

	ss << "\033[0m";

	std::string temp;
	ss >> temp;
	return temp;
}

