#pragma once
#include "GridManager.h"

class Agent;

class FSMState
{
protected:
	Vector2D startPos;
	Vector2D targetPos;

public:
	virtual void Enter(Agent* _agent) = 0;
	virtual void Exit(Agent* _agent) = 0;
	virtual FSMState* Update(Agent* _agent, float _dtime) = 0;
	virtual FSMState* ChangeStateCondition(Agent* _agent) = 0;
};