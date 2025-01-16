#pragma once

#include "Scene.h"
#include "BlackboardData.h"

const float MAX_DISTANCE_IS_VISIBLE = 10.f;

class SensorySystem
{
private: 
	Scene* world;

	BlackboardData blackboardData;

public:

	SensorySystem() = default;
	~SensorySystem() = default;

	void Update(Vector2D currentPos, Vector2D lastPlayerPosSaw,float dtime);

	inline BlackboardData GetBlackboardData() const { return blackboardData; }
};