#include "PathFindingAlgorithm.h"

PathFindingAlgorithm::PathFindingAlgorithm(Grid* _grid, std::vector<Agent*> agent)
{
	start = nullptr;
	goal = nullptr;
	elapsedTime = 0;
	timeToExecuteAlgorithm = 0.01f;
	goalReached = false;
	grid = _grid;
	agents = agent;
}

void PathFindingAlgorithm::InitPath()
{
	current = goal;
	path.push_back(current);
}

void PathFindingAlgorithm::ResetNodes()
{
	goalReached = false;
	frontierQueue = std::queue<Node*>();
	frontierQueuePriority = std::priority_queue<std::pair<Node*, int>, std::vector<std::pair<Node*, int>>, PriorityQueueComparator>();
	nodes.clear();
	cameFrom.clear();
	current = nullptr;
	path.clear();
	costSoFar.clear();
	agents[0]->clearPath();
	system("cls");
}

void PathFindingAlgorithm::RecoverPath()
{
	if (goalReached)
	{
		if (*current != *start)
		{
			// Miramos el path del goal -> start 
			for (Connection* conn : cameFrom)
			{
				// Si el connection actual es igual al current, añadimos un nuevo valor al path para el Agent
				if (*conn->getNodeTo() == *current)
				{
					path.push_back(conn->getNodeTo());
					current = conn->getNodeFrom();
					break;
				}
			}
		}
		else
		{
			std::reverse(path.begin(), path.end());

			// Mueve el agente con las posiciones por pixeles del mapa
			for (Node* node : path)
			{
				Vector2D cellPosition = grid->cell2pix(Vector2D(node->getX(), node->getY()));
				agents[0]->addPathPoint(cellPosition);
			}

			path.clear();
		}
	}
}

void PathFindingAlgorithm::ExecuteAlgorithm(Node* _startNode, Node* _goalNode)
{
	ResetNodes();
	SetStart(_startNode);
	SetGoal(_goalNode);
	InitFind();
}


void PathFindingAlgorithm::Draw()
{
	for (Node* node : nodes)
	{
		Vector2D position = grid->cell2pix(Vector2D(node->getX(), node->getY()));
		node->draw(position);
	}
}

void PathFindingAlgorithm::Update(float dt)
{
	if(goalReached == false)
		FindPath(dt);

	RecoverPath();
}


