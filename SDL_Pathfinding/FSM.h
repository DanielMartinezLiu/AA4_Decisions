#pragma once
#include "FSMState.h"

class FSM
{
private:

	FSMState* currentState;
public:
	FSM(Agent* _agent);

	void Update(Agent* _agent, float _dtime);
	void ChangeState(FSMState* _newState, Agent* _agent);
};

