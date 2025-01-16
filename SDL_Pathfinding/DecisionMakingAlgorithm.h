#pragma once
#include "Agent.h"
class DecisionMakingAlgorithm
{
protected:
	virtual void Update(Agent* _agent, float _dtime) = 0;
};

