#pragma once

#include "Node.h"
class Connection
{
private:
	Node* nodeFrom;
	Node* nodeTo;
	float weight;

public:

	Connection(Node* _nodeFrom, Node* _nodeTo, float _weight)
		:nodeFrom(_nodeFrom), nodeTo(_nodeTo), weight(_weight) {}

	Node* getNodeFrom() { return nodeFrom; }
	Node* getNodeTo() { return nodeTo; }
	float getWeight() { return weight; }
};