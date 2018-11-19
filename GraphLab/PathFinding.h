#pragma once
#include <vector>
#include "Vector2D.h"
#include "GridCell.h"
#include <list>
#include <iostream>
#include <map>

class PathFinding
{
public:
	PathFinding();
	~PathFinding();

	void addCell(GridCell *cell);
	void ClearOpenList() { m_openList.clear(); }
	void ClearVisitedList() { m_visitedList.clear(); }
	void ClearPathToGoal() { m_pathToGoal.clear(); }
	bool m_initlizedStartGoal;
	bool m_foundGoal;
	void updateStart(GridCell *start);
	void updateGoal(GridCell *goal);
	void wavefrontAlgorithm();
	void createAdjecancySets();

	GridCell *m_startCell;
	GridCell *m_goalCell;
	std::vector<GridCell*> m_openList;
	std::vector<GridCell*> m_visitedList;
	std::vector<Vector2D*> m_pathToGoal;
	const float m_cellOffset = 7.5f;

	GridCell *m_previousStartCell;
	GridCell *m_previousGoalCell;
	const int max_grid_size = 735;
	const int up = -50;
	const int top_right = -49;
	const int right = 1;
	const int bottom_right = 51;
	const int bottom = 50;
	const int bottom_left = 49;
	const int left = -1;
	const int top_left = -51;

};

