#include "PathFindingAStar.h"

void PathFindingAStar::InitFind()
{
	frontierQueuePriority.push({ start, 0 });
	cameFrom.push_back(new Connection(start, start, 0));
	nodes.push_back(frontierQueuePriority.top().first);
	costSoFar[start] = 0;
}

void PathFindingAStar::FindPath(float dTime)
{
	// Si llegas a la meta, deja de pintar mapa
	if (goalReached || frontierQueuePriority.empty())
		return;

	// Suma el tiempo para dibujar el mapa
	elapsedTime += dTime;

	if (elapsedTime > timeToExecuteAlgorithm)
	{
		Node* _current = frontierQueuePriority.top().first;
		frontierQueuePriority.pop();


		// Si el mapa ya se ha pintado completamente, iniciamos el camino
		if (*_current == *goal)
		{
			goalReached = true;
			InitPath();
			return;
		}

		// Comprobamos los vecinos
		for (Node* next : grid->getNeighbours(_current))
		{

			float newCost = costSoFar[_current] + grid->getTerrain(new Vector2D(next->getX(), next->getY()));
			if ((costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next]) && next->getType() != 0)
			{
				nodes.push_back(next);
				std::cout << "Added Node: X -> " << next->getX() << " Y -> " << next->getY() << " Type -> " << next->getType() << ". Node current size: " << nodes.size() << std::endl;

				costSoFar[next] = newCost;
				int priority = newCost + grid->getCost(goal, next);
				frontierQueuePriority.push({ next, priority });
				cameFrom.push_back(new Connection(_current, next, priority));
			}
		}
		elapsedTime = 0;
	}
}

