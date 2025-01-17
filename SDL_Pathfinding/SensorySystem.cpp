#include "SensorySystem.h"
#include "PlayerManager.h"
#include "GridManager.h"

void SensorySystem::Update(Vector2D currentPos, Vector2D velocity, Vector2D lastPlayerPosSaw, float dtime)
{
	blackboardData.hasGun = PLAYER_MANAGER.GetPlayer()->GetHasGun();
	blackboardData.lastPlayerPosSaw = lastPlayerPosSaw;

	int coneValue = Vector2DUtils::NewIsInsideCone(PLAYER_MANAGER.GetPlayer()->getPosition(), currentPos, velocity, 300, 75, 200, 150, 30, 90);

	if (coneValue > 0)
	{
		blackboardData.isVisible = true;
		blackboardData.coneValue = coneValue;
	}
	else
		blackboardData.isVisible = false;

	if (!LineOfSight(currentPos, PLAYER_MANAGER.GetPlayer()->getPosition()))
		blackboardData.isVisible = false;
}

bool SensorySystem::LineOfSight(const Vector2D& start, const Vector2D& end)
{
	Vector2D startPosition = GRID_MANAGER.GetGrid()->pix2cell(start);
	Vector2D endPosition = GRID_MANAGER.GetGrid()->pix2cell(end);

	int dx = abs(endPosition.x - startPosition.x);
	int dy = abs(endPosition.y - startPosition.y);
	int sx = (startPosition.x < endPosition.x) ? 1 : -1;
	int sy = (startPosition.y < endPosition.y) ? 1 : -1;
	int err = dx - dy;

	while(true) {
		Vector2D currentCell(startPosition.x, startPosition.y);

		if (!GRID_MANAGER.GetGrid()->isValidCell(currentCell) || !(GRID_MANAGER.GetGrid()->getNodes()[startPosition.y][startPosition.x]->getType() == 1)) {
			return false;
		}

		if (startPosition.x == endPosition.x && startPosition.y == endPosition.y)
			break;

		int e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			startPosition.x += sx;
		}
		if (e2 < dx) {
			err += dx;
			startPosition.y += sy;
		}
	}

	return true;
	
}
