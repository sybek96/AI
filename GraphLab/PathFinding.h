#pragma once
#include <vector>
#include "Vector2D.h"
#include "GridCell.h"
#include <list>
#include <iostream>

class PathFinding
{
public:
	PathFinding();
	~PathFinding();

	void FindPath(Vector2D currentPos, Vector2D targetPos);
	Vector2D NextPathPos(Vector2D pos, float radius);
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
//private:
	void SetStartAndGoal(GridCell start, GridCell goal);
	void PathOpened(int x, int y, float newCost, GridCell *parent);
	GridCell *GetNextCell();
	void ContinuePath();

	GridCell *m_startCell;
	GridCell *m_goalCell;
	std::vector<GridCell*> m_openList;
	std::vector<GridCell*> m_visitedList;
	std::vector<Vector2D*> m_pathToGoal;


	GridCell *m_previousStartCell;
	GridCell *m_previousGoalCell;

};

