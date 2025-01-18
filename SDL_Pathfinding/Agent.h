#pragma once
#include <iostream>
#include <minmax.h>
#include <SDL.h>
#include <SDL_image.h>

#include "SDL_SimpleApp.h"
#include "Path.h"
#include "Vector2D.h"
#include "utils.h"
#include "Node.h"
#include "SensorySystem.h"
#include "Blackboard.h"


class FSM;
class PathFindingAlgorithm;

class Agent
{
public:
	class SteeringBehavior
	{
	public:
		SteeringBehavior() {};
		virtual ~SteeringBehavior() {};
		virtual void applySteeringForce(Agent* agent, float dtime) {};
	};
private:
	SteeringBehavior* steering_behaviour;
	Vector2D position;
	Vector2D target;

	// Pathfinding
	Path path;
	int currentTargetIndex;

	float mass;
	float orientation;
	float max_force;
	float max_velocity;

	SDL_Texture* sprite_texture;
	bool draw_sprite;
	int sprite_num_frames;
	int sprite_w;
	int sprite_h;

	bool isPlayer;

	SensorySystem* sensorySystem;
	Blackboard* blackboard;
	FSM* stateMachine;
	PathFindingAlgorithm* currentPathfindingAlgorithm;

	bool hasGun;

public:
	Agent(bool _isPlayer);
	~Agent();
	Vector2D velocity;

	Vector2D getPosition();
	Vector2D getTarget();
	Vector2D getVelocity();

	float getMaxVelocity();
	float getMaxForce();
	float getMass();

	void setBehavior(SteeringBehavior* behavior);
	void setPosition(Vector2D position);
	void setTarget(Vector2D target);
	void setVelocity(Vector2D velocity);
	void setMaxVelocity(float maxVelocity);
	void addPathPoint(Vector2D point);
	void setCurrentTargetIndex(int idx);
	void SetFSM(FSM* _FSM);
	void SetHasGun(bool _hasGun);

	int getCurrentTargetIndex();
	int getPathSize();
	PathFindingAlgorithm* GetAlgorithm() { return currentPathfindingAlgorithm; }

	Vector2D getPathPoint(int idx);

	void clearPath();
	void update(float dtime, SDL_Event* event);
	void draw();
	void SensorySystemBehavior(float dtime);

	bool getIsPlayer() { return isPlayer; }
	void resetPath();

	bool Agent::loadSpriteTexture(char* filename, int num_frames = 1);

	bool GetHasGun() { return hasGun; }
	Blackboard* GetBlackBoard() { return blackboard; }

	void SetRandomPosition();
};