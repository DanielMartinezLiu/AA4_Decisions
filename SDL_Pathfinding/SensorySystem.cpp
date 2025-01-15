#include "SensorySystem.h"

void SensorySystem::Update(Vector2D currentPos, Vector2D lastPlayerPosSaw, float dtime)
{
	blackboardData.lastPlayerPosSaw = lastPlayerPosSaw;
}
