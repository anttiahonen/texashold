#ifndef HOLDEM_STATE
#define HOLDEM_STATE



class State
{
public:
	
	virtual ~State()
	{ }

	virtual void enter() = 0;

	virtual void execute() = 0;

	void executeRound();

	virtual void exit() = 0;

private:
	static State* instance;
};

#endif
