#pragma once
#include "FSMState.h"

class FSMState_Patroll : public FSMState
{
private:

	Vector2D SetRandomTarget();

	void ArriveTarget(Agent* _agent);

public:
	void Enter(Agent* _agent) override;
	void Exit(Agent* _agent) override;
	FSMState* Update(Agent* _agent, float _dtime) override;
	FSMState* ChangeStateCondition(Agent* _agent) override;

};

