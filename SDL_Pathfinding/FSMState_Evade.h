#pragma once
#include "FSMState.h"

class FSMState_Evade : public FSMState
{
public:
	void Enter(Agent* _agent) override;
	void Exit(Agent* _agent) override;
	FSMState* Update(Agent* _agent, float _dtime) override;
};

