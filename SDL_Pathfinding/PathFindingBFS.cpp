#include "PathFindingBFS.h"

void PathFindingBFS::InitFind()
{
	frontierQueue.push(start);
	cameFrom.push_back(new Connection(start, start, 0));
	nodes.push_back(frontierQueue.front());
}


void PathFindingBFS::FindPath(float dTime)
{
	// Si llegas a la meta, deja de pintar mapa
	if (goalReached || frontierQueue.empty())
		return;
	
	// Suma el tiempo para dibujar el mapa
	elapsedTime += dTime;

	if (elapsedTime > timeToExecuteAlgorithm)
	{
		Node* _current = frontierQueue.front();
		frontierQueue.pop();

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

			bool alreadyVisited = false;
			for (Connection* conn : cameFrom) 
			{
				// Comprobamos que connection, la siguiente sea el next
				if (conn->getNodeTo() == next) 
				{
					alreadyVisited = true;
					break;
				}
			}

			// Si no lo hemos visitado, añadimos un nuevo connection con el valor actual y el siguiente
			if (!alreadyVisited && next->getType() != 0)
			{
				nodes.push_back(next);
				std::cout << "Added Node: X -> " << next->getX() << " Y -> " << next->getY() << " Type -> " << next->getType() << ". Node current size: " << nodes.size() << std::endl;

				frontierQueue.push(next);
				cameFrom.push_back(new Connection(_current, next, 0)); 
			}
		}
		elapsedTime = 0;
	}
}
