#include "Grid.h"

using namespace std;

Grid::Grid(char* filename)
{
	num_cell_x = SRC_WIDTH / CELL_SIZE;
	num_cell_y = SRC_HEIGHT / CELL_SIZE;
	
	int x = 0;
	int y = 0;

	// Initialize the terrain matrix from file (for each cell a zero value indicates it's a wall, positive values indicate terrain cell cost)
	std::ifstream infile(filename);
	std::string line;
	while (std::getline(infile, line))
	{
		std::vector<Node*> node_row;
		std::stringstream lineStream(line);
		std::string cell;

		x = 0;
		while (std::getline(lineStream, cell, ','))
		{
			node_row.push_back(new Node(x, y, atoi(cell.c_str())));
			x++;
		}
		y++;
		nodes.push_back(node_row);
	}
	infile.close();
}

Grid::~Grid()
{
}

Vector2D Grid::cell2pix(Vector2D cell)
{
	int offset = CELL_SIZE / 2;
	return Vector2D(cell.x*CELL_SIZE + offset, cell.y*CELL_SIZE + offset);
}

Vector2D Grid::pix2cell(Vector2D pix)
{
	return Vector2D((float)((int)pix.x / CELL_SIZE), (float)((int)pix.y / CELL_SIZE));
}

bool Grid::isValidCell(Vector2D cell)
{
	if ((cell.x < 0) || (cell.y < 0) || (cell.y >= num_cell_y) || (cell.x >= num_cell_x))
		return false;
	return !(nodes[(unsigned int)cell.y][(unsigned int)cell.x]->getType() == 0);
}

int Grid::getTerrain(Vector2D* position)
{
	int x = position->x;
	int y = position->y;

	if (nodes[y][x] != nullptr)
	{

		if (x < 0 || y < 0 || x >= num_cell_x || y >= num_cell_y)
		{
			return 0;
		}

		return nodes[y][x]->getType();
	}
	return 0;
}

void Grid::changeWeight(Vector2D position)
{

	Node* currentNode = nodes[position.y][position.x];
	currentNode->setType(20);

	std::vector<Node*> firstNeighbourNodes = getNeighbours(currentNode);
	for (Node* firstNode : firstNeighbourNodes)
	{
		if (firstNode->getType() != 0)
		{
			firstNode->setType(15);
		}
		std::vector<Node*> secondNeighbourNodes = getNeighbours(firstNode);
		for (Node* secondNode : secondNeighbourNodes)
		{
			if (secondNode->getType() != 0 && secondNode != currentNode)
			{
				secondNode->setType(10);
			}
		}
		secondNeighbourNodes.clear();
	}
	firstNeighbourNodes.clear();
}

void Grid::resetWeight()
{
	for (std::vector<Node*> node : nodes)
	{
		for (Node* currentNode : node)
		{
			currentNode->setType(currentNode->getDefaultType());
		}
	}
}

int Grid::getCost(Node* current, Node* next)
{
	return sqrt(pow(current->getX() - next->getX(), 2) + pow(current->getY() - next->getY(), 2));
}

std::vector<Node*> Grid::getNeighbours(Node* currentNode)
{
	std::vector<Node*> neighbours;

	if (currentNode->getType() != 0)
	{
		if (currentNode->getY() > 0)
			neighbours.push_back(nodes[currentNode->getY() - 1][currentNode->getX()]);
		if (currentNode->getY() < num_cell_y - 1)
			neighbours.push_back(nodes[currentNode->getY() + 1][currentNode->getX()]);
		if (currentNode->getX() > 0)
			neighbours.push_back(nodes[currentNode->getY()][currentNode->getX() - 1]);
		if (currentNode->getX() < num_cell_x - 1)
			neighbours.push_back(nodes[currentNode->getY()][currentNode->getX() + 1]);
	}

	return neighbours;
}