#include "FSMState_Patroll.h"
#include "FSMState_Chase.h"
#include "FSMState_Evade.h"
#include "PathFindingAlgorithm.h"
#include "PlayerManager.h"

void FSMState_Chase::Enter(Agent* _agent)
{
	startPos = GRID_MANAGER.GetGrid()->pix2cell(Vector2D(_agent->getPosition().x, _agent->getPosition().y));
	targetPos = GRID_MANAGER.GetGrid()->pix2cell(Vector2D(PLAYER_MANAGER.GetPlayer()->getPosition().x, PLAYER_MANAGER.GetPlayer()->getPosition().y));

	_agent->GetAlgorithm()->ExecuteAlgorithm(new Node(startPos.x, startPos.y, 1), new Node(targetPos.x, targetPos.y, 1));
}

void FSMState_Chase::Exit(Agent* _agent)
{
}

FSMState* FSMState_Chase::Update(Agent* _agent, float _dtime)
{
	return ChangeStateCondition(_agent);
}

FSMState* FSMState_Chase::ChangeStateCondition(Agent* _agent)
{
	if (!_agent->GetBlackBoard()->GetIsVisible())
	{
		return new FSMState_Patroll();
	}

	if (_agent->GetBlackBoard()->GetHasGun())
	{
		return new FSMState_Evade();
	}

	return nullptr;
}
