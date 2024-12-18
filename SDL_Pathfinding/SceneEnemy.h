#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>

#include "Scene.h"
#include "Agent.h"
#include "Seek.h"
#include "PathFollowing.h"
#include "Grid.h"
#include "PathFindingAlgorithm.h"

class SceneEnemy : public Scene
{
public:
	SceneEnemy();
	~SceneEnemy();
	void update(float dtime, SDL_Event* event);
	void draw();
	const char* getTitle();
private:
	std::vector<Agent*> agents;
	Vector2D coinPosition;

	PathFindingAlgorithm* currentPathfindingAlgorithm;

	Grid* currentMaze;
	Vector2D* nodePosition;

	bool draw_grid;

	void CreateEnemy(std::vector<Node*> enemyPositions);

	void drawMaze(Grid* _grid);
	void drawCoin();

	void setColor(int r, int g, int b, Vector2D pos);

	SDL_Texture* background_texture;
	SDL_Texture* coin_texture;

	bool loadTextures(char* filename_bg, char* filename_coin);
};

