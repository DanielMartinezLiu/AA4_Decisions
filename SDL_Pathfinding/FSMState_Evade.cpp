#include "FSMState_Patroll.h"
#include "FSMState_Chase.h"
#include "FSMState_Evade.h"
#include "PathFindingAlgorithm.h"
#include "PlayerManager.h"
#include <random>

void FSMState_Evade::Enter(Agent* _agent)
{
    speed = 200;
    EvadeTarget(_agent);
    _agent->setMaxVelocity(speed);
}

void FSMState_Evade::Exit(Agent* _agent)
{
}

FSMState* FSMState_Evade::Update(Agent* _agent, float _dtime)
{
	return ChangeStateCondition(_agent);
}

FSMState* FSMState_Evade::ChangeStateCondition(Agent* _agent)
{
	if (GRID_MANAGER.GetGrid()->pix2cell(_agent->getPosition()) == targetPos)
	{
		return new FSMState_Patroll();
	}
	if (!_agent->GetBlackBoard()->GetHasGun())
	{
		return new FSMState_Chase();
	}

	return nullptr;
}
void FSMState_Evade::EvadeTarget(Agent* _agent)
{
    Vector2D playerPosition = PLAYER_MANAGER.GetPlayer()->getPosition();
    Vector2D agentPosition = _agent->getPosition();

    Vector2D directionToPlayer = playerPosition - agentPosition;
    float magnitude = sqrtf(directionToPlayer.x * directionToPlayer.x + directionToPlayer.y * directionToPlayer.y);
    if (magnitude > 0)
    {
        directionToPlayer /= magnitude;
    }
    else
    {
        directionToPlayer = Vector2D();
    }

    Vector2D oppositeDirection = directionToPlayer * -1;

    const int maxAttempts = 10;  
    const float minDist = 100.0f; 
    const float maxDist = 400.0f; 

    float dist = maxDist;
    Vector2D newTarget;

    for (int attempt = 0; attempt < maxAttempts; ++attempt)
    {
        newTarget = agentPosition + oppositeDirection * dist;

        float maxX = GRID_MANAGER.GetGrid()->getNumCellX() * GRID_MANAGER.GetGrid()->getNumCellX();
        float maxY = GRID_MANAGER.GetGrid()->getNumCellY() * GRID_MANAGER.GetGrid()->getNumCellY();

        float clampedX = (newTarget.x < 0.0f) ? 0.0f : (newTarget.x > maxX ? maxX : newTarget.x);
        float clampedY = (newTarget.y < 0.0f) ? 0.0f : (newTarget.y > maxY ? maxY : newTarget.y);

        newTarget = Vector2D(clampedX, clampedY);

        startPos = GRID_MANAGER.GetGrid()->pix2cell(agentPosition);
        targetPos = GRID_MANAGER.GetGrid()->pix2cell(newTarget);

        if (GRID_MANAGER.GetGrid()->isValidCell(targetPos))
        {
            break;
        }

        dist -= (maxDist - minDist) / maxAttempts;

        oppositeDirection.x += (rand() % 100 - 50) / 100.0f;
        oppositeDirection.y += (rand() % 100 - 50) / 100.0f;
        oppositeDirection.Normalize();
    }

    if (!GRID_MANAGER.GetGrid()->isValidCell(targetPos))
    {
        targetPos = GRID_MANAGER.GetGrid()->GetClosestValidCell(agentPosition);
    }

    _agent->GetAlgorithm()->ExecuteAlgorithm(new Node(startPos.x, startPos.y, 1), new Node(targetPos.x, targetPos.y, 1));
}



