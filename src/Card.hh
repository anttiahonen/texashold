#ifndef HOLDEM_CARD
#define HOLDEM_CARD

#include <string>

class Card
{

public:
Card(size_t const color, size_t const value) : color(color), value(value)
{ }

std::string toString() const;

size_t const color;
size_t const value;

};

#endif
