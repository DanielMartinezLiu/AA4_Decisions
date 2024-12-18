#pragma once

#include "Vector2D.h"
#include "utils.h"

class Node
{
private:
	int x;
	int y;
	int type;
	int defaultType;

public:
	Node(int _x, int _y) 
		: x(_x), y(_y) {}

	Node(int _x, int _y, int _type)
		: x(_x), y(_y), type(_type), defaultType(_type){}

	int getX() const { return x; }
	int getY() const { return y; }
	int getType() const { return type; }
	int getDefaultType() const { return defaultType; }

	void setType(int _type) { type = _type; }

	void draw(Vector2D position);

	bool operator==(const Node& other) const {
		return (this->x == other.x) && (this->y == other.y);
	}

	bool operator!=(const Node& other) const {
		return !(*this == other);
	}
};