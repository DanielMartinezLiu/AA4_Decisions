#include "Node.h"
#include "SDL_SimpleApp.h"

void Node::draw(Vector2D position)
{
	draw_circle(TheApp::Instance()->getRenderer(), (int)position.x, (int)position.y, 15, 255, 0, 0, 255);
}
