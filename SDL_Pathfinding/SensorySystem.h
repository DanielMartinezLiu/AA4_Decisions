#pragma once

#include "Scene.h"

// Referencia cruzada con agent, pero lo pide el profe asi en el UML
#include "Agent.h"

class SensorySystem
{
private: 
	Scene* world;
public:
	void Update(Agent* agent, float dtime);
};