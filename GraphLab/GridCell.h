#pragma once
#include <math.h>
#include "Vector2D.h"
#include <list>
#define WORLD_SIZE 50

struct GridCell
{
public:
	int m_xCoord;
	int m_yCoord;

	int m_id;
	GridCell *parent;
	Vector2D m_direction;
	float G; //accumulated distance from start node
	float H = 99999.0f; //heuristic estimate to goald
	bool blocked = false;
	bool isGoal = false;
	bool isStart = false;
	bool visited = false;
	std::vector<GridCell*> adjecancySet;

	GridCell() : parent(0) {};

	GridCell(int x, int y, GridCell* _parent = 0) : m_xCoord(x), m_yCoord(y),
		parent(_parent), m_id(x * WORLD_SIZE + y), G(0), H(0) {
		m_direction = Vector2D(0, 0);
	};

	//G + H (
	float GetF() { return G + H; }

	//current to target x + y
	float ManhattanDist(GridCell* endCell)
	{
		float x = static_cast<float>(fabs(this->m_xCoord - static_cast<float>(endCell->m_xCoord)));
		float y = static_cast<float>(fabs(this->m_yCoord - static_cast<float>(endCell->m_yCoord)));
		return x + y;
	}
};