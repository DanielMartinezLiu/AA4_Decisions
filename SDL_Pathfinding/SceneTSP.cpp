#include "SceneTSP.h"
#include "PathFindingAStar.h"
#include "PlayerManager.h"
#include "GridManager.h"
#include "FSM.h"

using namespace std;

SceneTSP::SceneTSP()
{
	draw_grid = false;
	currentMaze = new Grid("../res/maze.csv");
	nodePosition = new Vector2D;

	srand((unsigned int)time(NULL));

	GRID_MANAGER.SetGrid(currentMaze);

	CreateAgents(true, 200);
	CreateAgents(false, 50);
	CreateAgents(false, 50);

	PLAYER_MANAGER.SetPlayer(agents[0]);

	loadTextures("../res/maze.png", "../res/coin.png");
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
	switch (event->type) {
	case SDL_KEYDOWN:
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
			draw_grid = !draw_grid;
		if (event->key.keysym.scancode == SDL_SCANCODE_1)
			PLAYER_MANAGER.GetPlayer()->SetHasGun(!PLAYER_MANAGER.GetPlayer()->GetHasGun());
		break;
	case SDL_MOUSEBUTTONDOWN:
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			Vector2D startPos = currentMaze->pix2cell(Vector2D(PLAYER_MANAGER.GetPlayer()->getPosition().x, PLAYER_MANAGER.GetPlayer()->getPosition().y));
			Vector2D cell = currentMaze->pix2cell(Vector2D((float)(event->button.x), (float)(event->button.y)));

			if (currentMaze->isValidCell(cell))
			{
				PLAYER_MANAGER.GetPlayer()->GetAlgorithm()->ExecuteAlgorithm(new Node(startPos.x, startPos.y, 1), new Node(cell.x, cell.y, 1));
			}
		}
		break;
	default:
		break;
	}
	for (Agent* agent : agents)
	{
		agent->update(dtime, event);
	}
}

void SceneTSP::draw()
{
	drawMaze(currentMaze);

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
	return "SDL Path Finding :: Decision Making";
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

void SceneTSP::CreateAgents(bool isPlayer, int velocity)
{
	Agent* agent = new Agent(isPlayer);
	if (!isPlayer)
		agent->SetFSM(new FSM(agent));
	else
		agent->SetHasGun(true);
	agent->loadSpriteTexture("../res/soldier.png", 4);
	agent->setBehavior(new PathFollowing);
	agent->setTarget(Vector2D(-20, -20));
	agent->setMaxVelocity(velocity);
	agents.push_back(agent);
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


