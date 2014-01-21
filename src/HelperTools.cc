#include "HelperTools.hh"

std::string lines[76154];

// These are used for generating the odds look-up tables

/*odds oddsP2(76154, std::vector<int>(10));
odds oddsP3(76154, std::vector<int>(10));
odds oddsP4(76154, std::vector<int>(10));
odds oddsP5(76154, std::vector<int>(10));
odds oddsP6(76154, std::vector<int>(10));
odds winOddsP2(76154, std::vector<int>(182));
odds winOddsP3(76154, std::vector<int>(182));
odds winOddsP4(76154, std::vector<int>(182));
odds winOddsP5(76154, std::vector<int>(182));
odds winOddsP6(76154, std::vector<int>(182));
std::vector<odds> pOdds(5, odds(76154, std::vector<int>(10)));
std::vector<odds> winOddsVector(5, odds(76154, std::vector<int>(182)));*/

void initCardDag()
{
	std::ifstream lookUp;
	lookUp.open ("carddag.thd");
	if (lookUp.is_open())
	{
		for (size_t i = 0; i < 76154; i++)
			std::getline(lookUp, lines[i]);
	}
	else
	{
		std::cout << "Could not open the carddag.thd, terminating game" << std::endl;
		lookUp.close();
		exit(1);
	}
	lookUp.close();
}


/*
void initOdds(int players, int times)
{
	// iniating the data structures
	srand(time(NULL));
	pOdds[0] = oddsP2;
	winOddsVector[0] = winOddsP2;
	pOdds[1] = oddsP3;
	winOddsVector[1] = winOddsP3;
	pOdds[2] = oddsP4;
	winOddsVector[2] = winOddsP4;
	pOdds[3] = oddsP5;
	winOddsVector[3] = winOddsP5;
	pOdds[4] = oddsP6;
	winOddsVector[4] = winOddsP6;
	
	int counter = 0;
	for (int i = 0; i < times; i++)
	{
		counter++;
		if (counter % 10000 == 0)
			std::cout << counter << std::endl;
		int winner;
		std::vector<Card*> p1Cards;
		std::vector<Card*> p2Cards;
		std::vector<Card*> p3Cards;
		std::vector<Card*> p4Cards;
		std::vector<Card*> p5Cards;
		std::vector<Card*> p6Cards;

		std::vector< std::vector<Card*> > pCards;
		pCards.push_back(p1Cards);
		pCards.push_back(p2Cards);
		pCards.push_back(p3Cards);
		pCards.push_back(p4Cards);
		pCards.push_back(p5Cards);
		pCards.push_back(p6Cards);

		int pValues[players][5];
		Deck newDeck = Deck();
		newDeck.shuffle();
		//preflop
		for (int i = 0; i < players; i++)
		{
			pCards[i].push_back(newDeck.drawCard());
			pCards[i].push_back(newDeck.drawCard());
			std::vector<int> vals = calculateHandValue(pCards[i]);
			pValues[i][0] = vals[0];
		}
		//flop
		Card* flopCard1 = newDeck.drawCard();
		Card* flopCard2 = newDeck.drawCard();
		Card* flopCard3 = newDeck.drawCard();
		for (int i = 0; i < players; i++)
		{
			pCards[i].push_back(flopCard1);
			pCards[i].push_back(flopCard2);
			pCards[i].push_back(flopCard3);
			std::vector<int> vals = calculateHandValue(pCards[i]);
			pValues[i][1] = vals[0];

		}
		//turn
		Card* turnCard = newDeck.drawCard();
		for (int i = 0; i < players; i++)
		{
			pCards[i].push_back(turnCard);
			std::vector<int> vals = calculateHandValue(pCards[i]);
			pValues[i][2] = vals[0];
		}
		//river
		Card* riverCard = newDeck.drawCard();
		int winnerEqVal;
		int winnerRow;
		for (int i = 0; i < players; i++)
		{
			pCards[i].push_back(riverCard);
			std::vector<int> vals = calculateHandValue(pCards[i]);
			pValues[i][3] = vals[0];

			int newEqVal = eqToInt(vals[1]);
			if (i == 0)
			{
				winnerEqVal = newEqVal;
				winnerRow = vals[0];
				winner = i;
			} 
			else if (newEqVal > winnerEqVal)
			{
				winnerEqVal = newEqVal;
				winnerRow = vals[0];
				winner = i;
			}
			else if (newEqVal == winnerEqVal)
			{
 				if (winnerRow < vals[0])
					winner = i;
			} 
			pValues[i][4] = newEqVal;
		}
		for (int i = 0; i < players; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				int rowVal = pValues[i][j];
				int eqVal = pValues[i][4];
				int preFlopRow = pValues[i][0] - 13;
				pOdds[players - 2][rowVal][eqVal]++;
				pOdds[players - 2][rowVal][0]++;
				//total count of hand & table cards
				winOddsVector[players - 2][rowVal][preFlopRow]++;
				
				if (i == winner)
				{
					//win count from hand & table
					winOddsVector[players - 2][rowVal][preFlopRow+91]++;
				}
			}
		}
	}
}

void writeOddsToFile(int playerNum)
{
	std::stringstream oddsFileName;
	oddsFileName << "odds" << playerNum << ".thd";
	std::string normOdds;
	oddsFileName >> normOdds;
	std::stringstream winFileName;
	winFileName << "winOdds" << playerNum << ".thd";
	std::string winOdds;
	winFileName >> winOdds;

	std::ofstream oddsFile;
	std::ofstream winOddsFile;
	oddsFile.open(normOdds);
	winOddsFile.open(winOdds);

	if (oddsFile.is_open() && winOddsFile.is_open())
	{
		for (int i = 0; i < 76154; i++)
		{
			std::stringstream ss;
			std::stringstream ss2;
			ss << i << "\t";
			ss2 << i << "\t";
			// writes the total hits of odds (eq-classes) 
			for (int j = 0; j < 10; j++)
			{
					ss << pOdds[playerNum-2][i][j];
					if (j < 9)
						ss << "\t";
					else
						ss << "\n";
			}
			// writes the hits of win-odds
			for (int k = 0; k < 182; k++)
			{
				ss2 << winOddsVector[playerNum-2][i][k];
				if (k < 181)
					ss2 << "\t";
				else
					ss2 << "\n";
			}
			oddsFile << ss.str();
			winOddsFile << ss2.str();
		}
		oddsFile.close();
		winOddsFile.close();
	}
	else
	{
		std::cout << "Could not open/create the necessary files, terminating program" << std::endl;
		oddsFile.close();
		winOddsFile.close();
		exit(1);
	}
}*/


odds readOddsFromFile(int rowVal, size_t playerNum)
{
	std::vector<int> currentOdds(10);
	std::vector<int> currentWinOdds(182);

	std::ifstream oddsFile;
	std::stringstream oddsName;
	oddsName << "odds" << playerNum << ".thd";
	oddsFile.open(oddsName.str());
	std::ifstream winOddsFile;
	std::stringstream winOddsName;
	winOddsName << "winOdds" << playerNum << ".thd";
	winOddsFile.open(winOddsName.str());
	if (oddsFile.is_open() && winOddsFile.is_open())
	{
		std::string rowLine;
		std::string rowLine2;
		for (int i = 0; i < rowVal+1; i++)
		{
			std::getline(oddsFile, rowLine);
			std::getline(winOddsFile, rowLine2);

			if (i == rowVal)
			{
				//reading and saving normal odds from file
				std::vector<std::string> rowValues;
				std::stringstream data(rowLine);
				std::string line;
				std::getline(data,line,'\t');
				while (std::getline(data,line,'\t'))
						rowValues.push_back(line);
				for (int j = 0; j < 10 ; j++)
				{
					std::stringstream temp;
					temp << rowValues[j];
					int tempInt;
					temp >> tempInt;
					currentOdds[j] = tempInt;

 				}
				//reading and saving win odds from file
				std::vector<std::string> rowValues2;
				std::stringstream data2(rowLine2);
				std::string line2;
				std::getline(data2,line2,'\t');
				while (std::getline(data2,line2,'\t'))
					rowValues2.push_back(line2);
				for (int k = 0; k < 182 ; k++)
				{
					std::stringstream temp;
					temp << rowValues2[k];
					int tempInt;
					temp >> tempInt;
					currentWinOdds[k] = tempInt;
				}

			}

		}
		oddsFile.close();
		winOddsFile.close();

		odds returnOdds;
		returnOdds.push_back(currentOdds);
		returnOdds.push_back(currentWinOdds);

		return returnOdds;
	}
	else
	{
		std::cout << "Could not load the odds-files, terminating system" << std::endl;
		oddsFile.close();
		winOddsFile.close();
		exit(1);
	}
}

std::vector<int> calculateHandValue(std::vector<Card*> cards)
{
	//sort the cards for the DAG
	std::sort(cards.begin(), cards.end(), compareCards);

	std::vector<size_t> cardValues(cards.size());

	std::vector<Card*> spad;
	std::vector<Card*> heart;
	std::vector<Card*> diam;
	std::vector<Card*> club;
	int spads = 0;
	int hearts = 0;
	int diams = 0;
	int clubs = 0;

	std::vector<Card*> flushCards = spad;
	//check for flushes
	for (size_t i = 0; i < cards.size(); i++)
	{
		cardValues[i] = cards[i]->value-2;
		if (cards[i]->color == SPADES)
		{
			spads++;
			spad.push_back(cards[i]);
			if (spads > 4)
				flushCards = spad;
		}
		if (cards[i]->color == HEARTS)
		{
			hearts++;
			heart.push_back(cards[i]);
			if (hearts > 4)
				flushCards = heart;
		}
		if (cards[i]->color == DIAMONDS)
		{
			diams++;
			diam.push_back(cards[i]);
			if (diams > 4)
				flushCards = diam;

		}
		if (cards[i]->color == CLUBS)
		{
			clubs++;
			club.push_back(cards[i]);
			if (clubs > 4)
				flushCards = club;
		}

	}
	int rowValue = 0;
	int currentBestValue = 0;
	int nextRow = cardValues[0];

	for (size_t i = 0; i < cards.size(); i++)
	{
		std::vector<std::string> rowValues;
		std::string rowLine = lines[nextRow];
		std::stringstream data(rowLine);
		std::string line;
		while (std::getline(data,line,'\t'))
			rowValues.push_back(line);
		//get next row if needed
		if (i < cards.size()-1)
		{
			std::istringstream converter(rowValues[cardValues[i+1]+1]);
			converter >> nextRow;
		}
		else
		{
			//valueting 7-cards
			if (cards.size() == 7)
			{
				int tempVal;
				std::istringstream converter(rowValues[15]);
				converter >> tempVal;
				currentBestValue = tempVal;
			}
			//valueting less than 7-cards
			else
			{
				int stepper = 17;
				std::istringstream converter(rowValues[stepper]);
				converter >> currentBestValue;
			}
		}
	rowValue = nextRow;
	}
	// the same algorithm for getting correct value for flush-cases
	if (spads >= 5 || hearts >= 5 || diams >= 5 || clubs >= 5)
	{
		std::sort(flushCards.begin(), flushCards.end(), compareCards);
		std::vector<size_t> flushCardValues(flushCards.size());

		for (size_t i = 0; i < flushCards.size(); i++)
			flushCardValues[i] = flushCards[i]->value-2;

		int nextRow = flushCardValues[0];

		for (size_t i = 0; i < flushCards.size(); i++)
		{
			std::vector<std::string> rowValues;
			std::string rowLine = lines[nextRow];
			std::stringstream data(rowLine);
			std::string line;
			while (std::getline(data,line,'\t'))
				rowValues.push_back(line);
			if (i < flushCards.size()-1)
			{
				std::istringstream converter(rowValues[flushCardValues[i+1]+1]);
				converter >> nextRow;
			}
			else
			{
				int tempVal;
				std::istringstream converter(rowValues[14]);
				converter >> tempVal;
				currentBestValue = tempVal;
			}
		}
	}

	std::vector<int> returnTable(2);
	returnTable[0] = rowValue;
	returnTable[1] = currentBestValue;
	return returnTable;

}

std::vector<double> calculateOdds(int rowVal, size_t players, int handRowVal){
	std::vector<double> returnDoubles;
	odds oddsFromFile = readOddsFromFile(rowVal, players);

	double win = (double(oddsFromFile[1][(handRowVal-13)+91]) / double(oddsFromFile[1][handRowVal-13]))*100;
	double sf = (double(oddsFromFile[0][9]) / double(oddsFromFile[0][0]))*100;
	double four = (double(oddsFromFile[0][8]) / double(oddsFromFile[0][0]))*100;
	double fh = (double(oddsFromFile[0][7]) / double(oddsFromFile[0][0]))*100;
	double f = (double(oddsFromFile[0][6]) / double(oddsFromFile[0][0]))*100;
	double s = (double(oddsFromFile[0][5]) / double(oddsFromFile[0][0]))*100;
	double three = (double(oddsFromFile[0][4]) / double(oddsFromFile[0][0]))*100;
	double twop = (double(oddsFromFile[0][3]) / double(oddsFromFile[0][0]))*100;
	double onep = (double(oddsFromFile[0][2]) / double(oddsFromFile[0][0]))*100;
	double hc = (double(oddsFromFile[0][1]) / double(oddsFromFile[0][0]))*100;

	returnDoubles.push_back(win);
	returnDoubles.push_back(sf);
	returnDoubles.push_back(four);
	returnDoubles.push_back(fh);
	returnDoubles.push_back(f);
	returnDoubles.push_back(s);
	returnDoubles.push_back(three);
	returnDoubles.push_back(twop);
	returnDoubles.push_back(onep);
	returnDoubles.push_back(hc);

	return returnDoubles;
}

int eqToInt(int eq)
{
	int retEq;
	//straight-flush
	if (eq >= 1 && eq <= 10)
		retEq = 9;
	//four of a kind
	if (eq >= 11 && eq <= 166)
		retEq = 8;
	//full house
	if (eq >= 167 && eq <= 322)
		retEq = 7;
	//flush
	if (eq >= 323 && eq <= 1599)
		retEq = 6;
	//straight
	if (eq >= 1600 && eq <= 1609)
		retEq = 5;
	//three of a kind
	if (eq >= 1610 && eq <= 2467)
		retEq = 4;
	//two pairs
	if (eq >= 2468 && eq <= 3325)
		retEq = 3;
	//one pair
	if (eq >= 3326 && eq <= 6185)
		retEq = 2;
	//high card
	if (eq >= 6186 && eq <= 7462)
		retEq = 1;

	return retEq;
}

bool compareCards(Card* Card1, Card* Card2)
{
	return Card2->value < Card1->value;
}

std::string cardsToString(std::vector<Card*> cards)
{
	std::stringstream ss;

	for (size_t i = 0; i < cards.size(); i++)
	{
		ss << cards[i]->toString();

		if (i+1 != cards.size())
			ss << " ";
	}

	std::string temp;

	std::getline(ss, temp);

	return temp;
}
