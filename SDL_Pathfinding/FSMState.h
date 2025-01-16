#pragma once
#include "Agent.h"
class FSMState
{
public:

	virtual void Enter(Agent* _agent) = 0;
	virtual void Exit(Agent* _agent) = 0;
	virtual FSMState* Update(Agent* _agent, float _dtime) = 0;
};

