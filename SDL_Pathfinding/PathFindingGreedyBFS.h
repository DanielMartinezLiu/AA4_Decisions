#pragma once
#include <queue>
#include <unordered_map>
#include <memory>

#include "PathFindingAlgorithm.h"
#include "Node.h"
#include "Connection.h"

class PathFindingGreedyBFS : public PathFindingAlgorithm
{
public:
	PathFindingGreedyBFS(Grid* grid, std::vector<Agent*> agent) : PathFindingAlgorithm(grid, agent) {}

	void InitFind();

	virtual void FindPath(float dTime) override;
};

