#ifndef HOLDEM_STATEMACHINE
#define HOLDEM_STATEMACHINE

#include "GameStates.hh"
#include "thdefs.hh"

class StateMachine
{
public:
	static StateMachine* getInstance();
	~StateMachine();

	void update();

	void changeState(State* state);

	State* getCurrentState()
	{
		return currentState;
	}

private:
	StateMachine();
	static StateMachine* instance;

	State* currentState;
};

#endif
