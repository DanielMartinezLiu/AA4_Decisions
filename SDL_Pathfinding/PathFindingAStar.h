#pragma once
#include <queue>
#include <unordered_map>

#include "PathFindingAlgorithm.h"
#include "Node.h"
#include "Connection.h"

class PathFindingAStar : public PathFindingAlgorithm
{
public:
	PathFindingAStar(Grid* grid, std::vector<Agent*> agent) : PathFindingAlgorithm(grid, agent) {}
	virtual void InitFind() override;

	virtual void FindPath(float dTime) override;
};