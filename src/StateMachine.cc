#include "StateMachine.hh"

StateMachine* StateMachine::instance;

StateMachine* StateMachine::getInstance()
{
	if (!instance)
		instance = new StateMachine();

	return instance;
}

StateMachine::StateMachine()
{
	currentState = PreFlop::getInstance();

	currentState->enter();
}

StateMachine::~StateMachine()
{
	delete PreFlop::getInstance();
	delete Flop::getInstance();
	delete Turn::getInstance();
	delete River::getInstance();
	delete Win::getInstance();
	delete End::getInstance();
}

void StateMachine::update()
{
	currentState->execute();
}

void StateMachine::changeState(State* state)
{
	currentState->exit();

	currentState = state;

	currentState->enter();
}
