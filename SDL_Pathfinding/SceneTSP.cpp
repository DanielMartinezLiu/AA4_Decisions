#include "SceneTSP.h"
#include "PathFindingAStar.h"

using namespace std;

SceneTSP::SceneTSP()
{
	draw_grid = false;
	currentMaze = new Grid("../res/maze1.csv");
	nodePosition = new Vector2D;

	srand((unsigned int)time(NULL));

	Agent* agent = new Agent(true);
	agent->loadSpriteTexture("../res/soldier.png", 4);
	agent->setBehavior(new PathFollowing);
	agent->setTarget(Vector2D(-20, -20));
	agents.push_back(agent);

	currentPathfindingAlgorithm = new PathFindingAStar(currentMaze, agents);
	currentPathfindingAlgorithm->SetTimeToExecuteAlgorithm(0);
	loadTextures("../res/maze.png", "../res/coin.png");

	// set agent position coords to the center of a random cell
	Vector2D rand_cell(-1, -1);
	while (!currentMaze->isValidCell(rand_cell))
		rand_cell = Vector2D((float)(rand() % currentMaze->getNumCellX()), (float)(rand() % currentMaze->getNumCellY()));
	agents[0]->setPosition(currentMaze->cell2pix(rand_cell));

	// set the coin in a random cell (but at least 3 cells far from the agent)
	
	for (int i = 0; i < 5; i++)
	{
		coinPositions.push_back(new Vector2D(-1,-1));
	}

	for (Vector2D* position : coinPositions)
	{
		while ((!currentMaze->isValidCell(*position)) || (Vector2D::Distance(*position, rand_cell) < 3))
			*position = Vector2D((float)(rand() % currentMaze->getNumCellX()), (float)(rand() % currentMaze->getNumCellY()));
	}
	
	Vector2D startPos = currentMaze->pix2cell(Vector2D(agents[0]->getPosition().x, agents[0]->getPosition().y));
	Node* startNode =  new Node(startPos.x, startPos.y);

	Vector2D* neareastCoinPos = ReturnNeareastCoin(startNode);
	Node* goalNode = new Node(neareastCoinPos->x, neareastCoinPos->y);
	currentPathfindingAlgorithm->ExecuteAlgorithm(startNode, goalNode);
}

SceneTSP::~SceneTSP()
{
	if (background_texture)
		SDL_DestroyTexture(background_texture);
	if (coin_texture)
		SDL_DestroyTexture(coin_texture);

	for (int i = 0; i < (int)agents.size(); i++)
	{
		delete agents[i];
	}

	for (Vector2D* position : coinPositions)
	{
		delete position;
	}
}

void SceneTSP::update(float dtime, SDL_Event* event)
{
	if (coinPositions.size() == 0)
	{
		return;
	}

	for (Agent* agent : agents)
	{
		agent->update(dtime, event);
	}

	currentPathfindingAlgorithm->Update(dtime);

	// if we have arrived to the coin, replace it in a random cell!
	for (int i = 0; i < coinPositions.size(); i++)
	{
		if ((agents[0]->getCurrentTargetIndex() == -1) && (currentMaze->pix2cell(agents[0]->getPosition()) == *coinPositions[i]))
		{
			coinPositions.erase(coinPositions.begin() + i);

			Vector2D startPos = currentMaze->pix2cell(Vector2D(agents[0]->getPosition().x, agents[0]->getPosition().y));
			Node* startNode = new Node(startPos.x, startPos.y);

			Vector2D* neareastCoinPos = ReturnNeareastCoin(startNode);
			Node* goalNode = new Node(neareastCoinPos->x, neareastCoinPos->y);

			if (coinPositions.size() == 0)
			{
				return;
			}
			currentPathfindingAlgorithm->ExecuteAlgorithm(startNode, goalNode);
		}
	}
	

	currentMaze->resetWeight();

	for (int i = 1; i < agents.size(); i++)
	{
		Vector2D position = currentMaze->pix2cell(Vector2D(agents[i]->getPosition()));
		currentMaze->changeWeight(position);
	}
}

void SceneTSP::draw()
{
	drawMaze(currentMaze);
	drawCoin();

	currentPathfindingAlgorithm->Draw();

	if (draw_grid)
	{
		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 255, 255, 255, 127);
		for (int i = 0; i < SRC_WIDTH; i += CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), i, 0, i, SRC_HEIGHT);
		}
		for (int j = 0; j < SRC_HEIGHT; j = j += CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), 0, j, SRC_WIDTH, j);
		}
	}

	for (Agent* agent : agents)
	{
		agent->draw();
	}
}

const char* SceneTSP::getTitle()
{
	return "SDL Path Finding :: PathFinding Enemy Demo";
}

void SceneTSP::drawMaze(Grid* _grid)
{
	SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 0, 155, 255, 255);

	for (int j = 0; j < _grid->getNumCellY(); j++)
	{
		for (int i = 0; i < _grid->getNumCellX(); i++)
		{
			Vector2D pos = _grid->cell2pix(Vector2D(i, j));
			nodePosition->x = i, nodePosition->y = j;

			if (!_grid->isValidCell(Vector2D((float)i, (float)j)))
			{
				setColor(97, 97, 97, pos);
			}
			else
			{
				switch (_grid->getTerrain(nodePosition))
				{
				case 1:
					setColor(168, 230, 163, pos);
					break;
				case 2:
					setColor(255, 245, 157, pos);
					break;
				case 3:
					setColor(255, 204, 128, pos);
					break;
				case 4:
					setColor(239, 154, 154, pos);
					break;
				case 10:
					setColor(0, 0, 255, pos);
					break;
				case 15:
					setColor(0, 255, 0, pos);
					break;
				case 20:
					setColor(255, 0, 0, pos);
					break;
				default:
					break;
				}
			}
		}
	}
}

void SceneTSP::drawCoin()
{
	for (Vector2D* position : coinPositions)
	{
		Vector2D coin_coords = currentMaze->cell2pix(*position);
		int offset = CELL_SIZE / 2;
		SDL_Rect dstrect = { (int)coin_coords.x - offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE };
		SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
	}
}

Vector2D* SceneTSP::ReturnNeareastCoin(Node* _startNode)
{
	Vector2D* neareastCoinPos = new Vector2D(100,100);

	for (Vector2D* position : coinPositions)
	{
		if (currentMaze->getCost(new Node(position->x, position->y), _startNode)
			< currentMaze->getCost(new Node(neareastCoinPos->x, neareastCoinPos->y), _startNode))
		{
			neareastCoinPos = position;
		}
	}

	return neareastCoinPos;
}

void SceneTSP::setColor(int r, int g, int b, Vector2D pos)
{
	SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), r, g, b, 255);
	Vector2D coords = pos - Vector2D((float)CELL_SIZE / 2, (float)CELL_SIZE / 2);
	SDL_Rect rect = { (int)coords.x, (int)coords.y, CELL_SIZE, CELL_SIZE };
	SDL_RenderFillRect(TheApp::Instance()->getRenderer(), &rect);
}

bool SceneTSP::loadTextures(char* filename_bg, char* filename_coin)
{
	SDL_Surface* image = IMG_Load(filename_bg);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	background_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	image = IMG_Load(filename_coin);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	coin_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	return true;
}


