#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>

#include "Agent.h"
#include "Node.h"

class Grid
{
public:
	Grid(char* filename);
	~Grid();

private:
	int num_cell_x;
	int num_cell_y;
	std::vector< std::vector<Node*> > nodes;

public:
	Vector2D cell2pix(Vector2D cell);
	Vector2D pix2cell(Vector2D pix);

	bool isValidCell(Vector2D cell);

	int getNumCellX() { return num_cell_x; }
	int getNumCellY() { return num_cell_y; }
	int getTerrain(Vector2D* position);

	void changeWeight(Vector2D position);
	void resetWeight();

	int getCost(Node* current, Node* next);

	std::vector< std::vector<Node*> > getNodes() { return nodes; }
	std::vector<Node*> getNeighbours(Node* currentNode);
}; 
