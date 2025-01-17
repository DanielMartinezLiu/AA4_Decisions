#pragma once

#include "Scene.h"
#include "BlackboardData.h"

class SensorySystem
{
private: 
	Scene* world;

	BlackboardData blackboardData;
	float distance = 300;
	float backwardRadius = 75;
	float forwardRadius = 200;
	float internalRadius = 150;
	float forwardAngle = 30;
	float visionAngle = 90;

public:

	SensorySystem() = default;
	~SensorySystem() = default;

	void Update(Vector2D currentPos, Vector2D velocity, Vector2D lastPlayerPosSaw, float dtime);
	bool LineOfSight(const Vector2D& start, const Vector2D& end);

	inline BlackboardData GetBlackboardData() const { return blackboardData; }
};