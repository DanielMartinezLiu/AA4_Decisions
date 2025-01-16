#pragma once
#include "FSMState.h"

class FSMState_Patroll : public FSMState
{
private:

	Vector2D SetRandomTarget();
	Vector2D startPos;
	Vector2D targetPos;

public:
	void Enter(Agent* _agent) override;
	void Exit(Agent* _agent) override;
	FSMState* Update(Agent* _agent, float _dtime) override;
};

