#include "FSMState_Patroll.h"
#include "FSMState_Chase.h"
#include "FSMState_Evade.h"
#include "PathFindingAlgorithm.h"
#include "PlayerManager.h"
#include <random>

void FSMState_Evade::Enter(Agent* _agent)
{
    EvadeTarget(_agent);
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
    // Obtener la posición actual del jugador y la del agente
    Vector2D playerPosition = PLAYER_MANAGER.GetPlayer()->getPosition();
    Vector2D agentPosition = _agent->getPosition();

    // Calcular la dirección hacia el jugador
    Vector2D directionToPlayer = (playerPosition - agentPosition).Normalize();

    // Obtener la dirección opuesta al jugador
    Vector2D oppositeDirection = Vector2D(directionToPlayer.x * -1, directionToPlayer.y * -1);

    float dist = rand() % 200 + 50;
    Vector2D newTarget = agentPosition + oppositeDirection.Normalize() * dist;

    float maxX = GRID_MANAGER.GetGrid()->getNumCellX() * GRID_MANAGER.GetGrid()->getNumCellX();
    float maxY = GRID_MANAGER.GetGrid()->getNumCellY() * GRID_MANAGER.GetGrid()->getNumCellY();

    // Asegurarte de que el nuevo objetivo está dentro de los límites del mapa (opcional, pero recomendable)
    float clampedX = newTarget.x < 0.0f ? 0.0f : (newTarget.x > maxX ? maxX : newTarget.x);
    float clampedY = newTarget.y < 0.0f ? 0.0f : (newTarget.y > maxY ? maxY : newTarget.y);

    newTarget = Vector2D(clampedX, clampedY);

    // Establece el nuevo objetivo y calcula la ruta hacia él
    startPos = GRID_MANAGER.GetGrid()->pix2cell(agentPosition);
    targetPos = GRID_MANAGER.GetGrid()->pix2cell(newTarget);

    if (!GRID_MANAGER.GetGrid()->isValidCell(targetPos))
    {
        EvadeTarget(_agent);
        return;
    }

    // Ejecuta el algoritmo de pathfinding para obtener la ruta hacia el nuevo objetivo
    _agent->GetAlgorithm()->ExecuteAlgorithm(new Node(startPos.x, startPos.y, 1), new Node(targetPos.x, targetPos.y, 1));
}