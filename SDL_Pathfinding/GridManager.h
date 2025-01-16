#pragma once
#include "Grid.h"

#define GRID_MANAGER GridManager::Instance()

class GridManager
{
private:

	GridManager() = default;

	GridManager(const GridManager&) = delete;
	GridManager& operator =(const GridManager&) = delete;

	Grid* grid;

public:

	inline static GridManager& Instance()
	{
		static GridManager _grid;
		return _grid;
	}

	inline Grid* GetGrid() const { return grid; }

	inline void SetGrid(Grid* _grid) { grid = _grid; }
};

