#pragma once
#include "FSMState.h"

class FSM
{
private:

	FSMState* currentState;

	void ChangeState(FSMState* _newState, Agent* _agent);

public:

	void Update(Agent* _agent, float _dtime);
};

