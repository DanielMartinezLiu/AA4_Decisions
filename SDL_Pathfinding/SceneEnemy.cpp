#include "SceneEnemy.h"
#include "PathFindingDijkstra.h"

using namespace std;

SceneEnemy::SceneEnemy()
{
	draw_grid = false;
	currentMaze = new Grid("../res/maze1.csv");
	nodePosition = new Vector2D;

	srand((unsigned int)time(NULL));

	std::vector<Node*> enemyPositions;
	enemyPositions.push_back(new Node(5, 1, 0));
	enemyPositions.push_back(new Node(10, 3, 0));
	enemyPositions.push_back(new Node(25, 3, 0));
	enemyPositions.push_back(new Node(27, 8, 0));
	enemyPositions.push_back(new Node(25, 10, 0));
	enemyPositions.push_back(new Node(15, 10, 0));
	enemyPositions.push_back(new Node(12, 5, 0));
	enemyPositions.push_back(new Node(7, 5, 0));

	std::vector<Node*> enemyPositions2;
	enemyPositions2.push_back(new Node(37, 20, 0));
	enemyPositions2.push_back(new Node(37, 10, 0));
	enemyPositions2.push_back(new Node(32, 10, 0));
	enemyPositions2.push_back(new Node(32, 14, 0));
	enemyPositions2.push_back(new Node(22, 14, 0));
	enemyPositions2.push_back(new Node(22, 20, 0));
	enemyPositions2.push_back(new Node(29, 20, 0));
	enemyPositions2.push_back(new Node(32, 18, 0));
	enemyPositions2.push_back(new Node(34, 20, 0));

	Agent* agent = new Agent(true);
	agent->loadSpriteTexture("../res/soldier.png", 4);
	agent->setBehavior(new PathFollowing);
	agent->setTarget(Vector2D(-20, -20));
	agents.push_back(agent);

	CreateEnemy(enemyPositions);
	CreateEnemy(enemyPositions2);

	currentPathfindingAlgorithm = new PathFindingDijkstra(currentMaze, agents);
	currentPathfindingAlgorithm->SetTimeToExecuteAlgorithm(0);
	loadTextures("../res/maze.png", "../res/coin.png");

	// set agent position coords to the center of a random cell
	Vector2D rand_cell(-1, -1);
	while (!currentMaze->isValidCell(rand_cell))
		rand_cell = Vector2D((float)(rand() % currentMaze->getNumCellX()), (float)(rand() % currentMaze->getNumCellY()));
	agents[0]->setPosition(currentMaze->cell2pix(rand_cell));

	// set the coin in a random cell (but at least 3 cells far from the agent)
	coinPosition = Vector2D(-1, -1);
	while ((!currentMaze->isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, rand_cell) < 3))
		coinPosition = Vector2D((float)(rand() % currentMaze->getNumCellX()), (float)(rand() % currentMaze->getNumCellY()));
}

SceneEnemy::~SceneEnemy()
{
	if (background_texture)
		SDL_DestroyTexture(background_texture);
	if (coin_texture)
		SDL_DestroyTexture(coin_texture);

	for (int i = 0; i < (int)agents.size(); i++)
	{
		delete agents[i];
	}
}

void SceneEnemy::update(float dtime, SDL_Event* event)
{
	/* Keyboard & Mouse events */
	switch (event->type) {
	case SDL_KEYDOWN:
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
			draw_grid = !draw_grid;
		break;
	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONDOWN:
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			Vector2D startPos = currentMaze->pix2cell(Vector2D(agents[0]->getPosition().x, agents[0]->getPosition().y));
			Vector2D cell = currentMaze->pix2cell(Vector2D((float)(event->button.x), (float)(event->button.y)));

			if (currentMaze->isValidCell(cell))
			{
				currentPathfindingAlgorithm->ExecuteAlgorithm(new Node(startPos.x, startPos.y, 1), new Node(cell.x, cell.y, 1));
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

	currentPathfindingAlgorithm->Update(dtime);

	// if we have arrived to the coin, replace it in a random cell!
	if ((agents[0]->getCurrentTargetIndex() == -1) && (currentMaze->pix2cell(agents[0]->getPosition()) == coinPosition))
	{
		coinPosition = Vector2D(-1, -1);
		while ((!currentMaze->isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, currentMaze->pix2cell(agents[0]->getPosition())) < 3))
			coinPosition = Vector2D((float)(rand() % currentMaze->getNumCellX()), (float)(rand() % currentMaze->getNumCellY()));
	}

	currentMaze->resetWeight(); 

	for (int i = 1; i < agents.size(); i++)
	{
		Vector2D position = currentMaze->pix2cell(Vector2D(agents[i]->getPosition()));
		currentMaze->changeWeight(position);
	}
}

void SceneEnemy::draw()
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

const char* SceneEnemy::getTitle()
{
	return "SDL Path Finding :: PathFinding Enemy Demo";
}

void SceneEnemy::CreateEnemy(std::vector<Node*> enemyPositions)
{
	Agent* enemy = new Agent(false);
	enemy->setMaxVelocity(10);
	for (Node* node : enemyPositions)
	{
		enemy->addPathPoint(currentMaze->cell2pix(Vector2D(node->getX(), node->getY())));
	}
	enemy->setPosition(currentMaze->cell2pix(Vector2D(enemyPositions[0]->getX(), enemyPositions[0]->getY())));
	enemy->loadSpriteTexture("../res/soldier.png", 4);
	enemy->setBehavior(new PathFollowing);
	enemy->setTarget(Vector2D(-20, -20));
	agents.push_back(enemy);
}

void SceneEnemy::drawMaze(Grid* _grid)
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

void SceneEnemy::drawCoin()
{
	Vector2D coin_coords = currentMaze->cell2pix(coinPosition);
	int offset = CELL_SIZE / 2;
	SDL_Rect dstrect = { (int)coin_coords.x - offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE };
	SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
}

void SceneEnemy::setColor(int r, int g, int b, Vector2D pos)
{
	SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), r, g, b, 255);
	Vector2D coords = pos - Vector2D((float)CELL_SIZE / 2, (float)CELL_SIZE / 2);
	SDL_Rect rect = { (int)coords.x, (int)coords.y, CELL_SIZE, CELL_SIZE };
	SDL_RenderFillRect(TheApp::Instance()->getRenderer(), &rect);
}

bool SceneEnemy::loadTextures(char* filename_bg, char* filename_coin)
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
