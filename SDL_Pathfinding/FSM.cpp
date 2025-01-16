#include "FSM.h"

void FSM::ChangeState(FSMState* _newState, Agent* _agent)
{
	currentState->Exit(_agent);
	currentState = _newState;
	currentState->Enter(_agent);
}

void FSM::Update(Agent* _agent, float _dtime)
{
	FSMState* newState = currentState->Update(_agent, _dtime);

	if (newState != nullptr)
		ChangeState(newState, _agent);
}
