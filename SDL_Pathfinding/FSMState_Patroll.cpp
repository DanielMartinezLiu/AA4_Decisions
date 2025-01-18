#include "FSMState_Patroll.h"
#include "FSMState_Chase.h"
#include "PathFindingAlgorithm.h"

Vector2D FSMState_Patroll::SetRandomTarget()
{
	Vector2D rand_cell(-1, -1);

	while (!GRID_MANAGER.GetGrid()->isValidCell(rand_cell))
		rand_cell = Vector2D((float)(rand() % GRID_MANAGER.GetGrid()->getNumCellX()), (float)(rand() % GRID_MANAGER.GetGrid()->getNumCellY()));

	return rand_cell;
}


void FSMState_Patroll::Enter(Agent* _agent)
{
	speed = 50;

	startPos = GRID_MANAGER.GetGrid()->pix2cell(Vector2D(_agent->getPosition().x, _agent->getPosition().y));
	targetPos = SetRandomTarget();

	_agent->GetAlgorithm()->ExecuteAlgorithm(new Node(startPos.x, startPos.y, 1), new Node(targetPos.x, targetPos.y, 1));

	_agent->setMaxVelocity(speed);
}

void FSMState_Patroll::Exit(Agent* _agent)
{
}

FSMState* FSMState_Patroll::Update(Agent* _agent, float _dtime)
{
	ArriveTarget(_agent);

    return ChangeStateCondition(_agent);
}

FSMState* FSMState_Patroll::ChangeStateCondition(Agent* _agent)
{
	if(_agent->GetBlackBoard()->GetConeValue() >= 30)
	{
		return new FSMState_Chase();
	}

	return nullptr;
}

void FSMState_Patroll::ArriveTarget(Agent* _agent)
{
	if (GRID_MANAGER.GetGrid()->pix2cell(_agent->getPosition()) == targetPos)
	{
		startPos = GRID_MANAGER.GetGrid()->pix2cell(Vector2D(_agent->getPosition().x, _agent->getPosition().y));
		targetPos = SetRandomTarget();

		_agent->GetAlgorithm()->ExecuteAlgorithm(new Node(startPos.x, startPos.y, 1), new Node(targetPos.x, targetPos.y, 1));	
	}
}




