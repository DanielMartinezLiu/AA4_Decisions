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

class SceneTSP : public Scene
{
public:
	SceneTSP();
	~SceneTSP();
	void update(float dtime, SDL_Event* event);
	void draw();
	const char* getTitle();
private:
	std::vector<Agent*> agents;
	std::vector<Vector2D*> coinPositions;
	bool firstExecute;

	PathFindingAlgorithm* currentPathfindingAlgorithm;

	Grid* currentMaze;
	Vector2D* nodePosition;

	bool draw_grid;

	void drawMaze(Grid* _grid);
	void drawCoin();

	Vector2D* ReturnNeareastCoin(Node* _startNode);

	void setColor(int r, int g, int b, Vector2D pos);

	SDL_Texture* background_texture;
	SDL_Texture* coin_texture;

	bool loadTextures(char* filename_bg, char* filename_coin);
};
