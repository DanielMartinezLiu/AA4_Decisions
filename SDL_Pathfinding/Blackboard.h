#pragma once
#include <unordered_map>
#include <string>

#include "BlackboardData.h"

class Blackboard
{
private:

	BlackboardData blackboardData;

public:

	Blackboard() = default;

	~Blackboard() = default;

	inline bool GetIsVisible() const { return blackboardData.isVisible; }
	inline bool GetHasGun() const { return blackboardData.hasGun; }
	inline Vector2D GetLastTimeSeenPos() const { return blackboardData.lastPlayerPosSaw; }

	inline BlackboardData SetBlackBoardData(const BlackboardData& _blackboardData) { return blackboardData = _blackboardData; }
};

