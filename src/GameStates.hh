#ifndef HOLDEM_GAMESTATES
#define HOLDEM_GAMESTATES

#include "State.hh"

class PreFlop : public State
{
public:
	static PreFlop* getInstance();

	virtual void enter();

	virtual void execute();

	virtual void exit();

	static int roundCounter;

private:
	PreFlop()
	{ }
	PreFlop(const PreFlop&);
	PreFlop& operator=(const PreFlop&);

	static PreFlop* instance;

};

class Flop : public State
{
public:
	static Flop* getInstance();

	virtual void enter();

	virtual void execute();

	virtual void exit();

private:
	Flop()
	{ }
	Flop(const Flop&);
	Flop& operator=(const Flop&);

	static Flop* instance;
};

class Turn : public State
{
public:
	static Turn* getInstance();

	virtual void enter();

	virtual void execute();

	virtual void exit();

private:
	Turn()
	{ }
	Turn(const Turn&);
	Turn& operator=(const Turn&);

	static Turn* instance;
};

class River : public State
{
public:
	static River* getInstance();

	virtual void enter();

	virtual void execute();

	virtual void exit();

private:
	River()
	{ }
	River(const River&);
	River& operator=(const River&);

	static River* instance;
};

class Win : public State
{
public:
	static Win* getInstance();

	virtual void enter();

	virtual void execute();

	virtual void exit();

private:
	Win()
	{ }
	Win(const Win&);
	Win& operator=(const Win&);

	static Win* instance;
};

class End : public State
{
public:
	static End* getInstance();

	virtual void enter();

	virtual void execute();

	virtual void exit();

private:
	End()
	{ }
	End(const End&);
	End& operator=(const End&);

	static End* instance;
};


#endif
