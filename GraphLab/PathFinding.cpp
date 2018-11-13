#include "PathFinding.h"



PathFinding::PathFinding()
{
	m_initlizedStartGoal = false;
	m_foundGoal = false;

}


PathFinding::~PathFinding()
{
}

void PathFinding::FindPath(Vector2D currentPos, Vector2D targetPos)
{
	//if (!m_initlizedStartGoal)
	//{
	//	//for (int i = 0; i < m_openList.size(); i++)
	//	//{
	//	//	delete m_openList[i];
	//	//}
	//	//m_openList.clear();
	//	//for (int i = 0; i < m_visitedList.size(); i++)
	//	//{
	//	//	delete m_visitedList[i];
	//	//}
	//	//m_visitedList.clear();
	//	//for (int i = 0; i < m_pathToGoal.size(); i++)
	//	//{
	//	//	delete m_pathToGoal[i];
	//	//}
	//	m_pathToGoal.clear();
	//	////Initialize start node
	//	//GridCell start;
	//	//start.m_xCoord = currentPos.x;
	//	//start.m_yCoord = currentPos.y;

	//	////initialze goal node
	//	//GridCell goal;
	//	//goal.m_xCoord = targetPos.x;
	//	//goal.m_yCoord = targetPos.y;

	//	//SetStartAndGoal(start, goal);
	//	m_initlizedStartGoal = true;
	//}
	//if (m_initlizedStartGoal)
	//{
	//	ContinuePath();
	//}
}

Vector2D PathFinding::NextPathPos(Vector2D pos, float radius)
{
	int index = 1;

	Vector2D nextPos;
	nextPos.x = m_pathToGoal[m_pathToGoal.size() - index]->x;
	nextPos.y = m_pathToGoal[m_pathToGoal.size() - index]->y;

	//pos is current position of AI
	Vector2D distance = nextPos - pos;

	if (index < m_pathToGoal.size())
	{
		//if AI touches first position in the path we erase it radius is AI radius
		if (distance.Length() < radius)
		{
			m_pathToGoal.erase(m_pathToGoal.end() - index);
		}
	}
	return nextPos;
}

void PathFinding::addCell(GridCell * cell)
{
	m_openList.push_back(cell);
}

void PathFinding::updateStart(GridCell * start)
{
	m_previousStartCell = m_startCell;
	if (m_previousStartCell != nullptr)
	{
		m_previousStartCell->isStart = false;
	}

	m_startCell = start;
	m_startCell->isStart = true;

}

void PathFinding::updateGoal(GridCell * goal)
{
	m_previousGoalCell = m_goalCell;
	if (m_previousGoalCell != nullptr)
	{
		m_previousGoalCell->isGoal = false;
	}

	m_goalCell = goal;
	m_goalCell->isGoal = true;
}

/// <summary>
/// This algorithm generates the heat map
/// </summary>
void PathFinding::wavefrontAlgorithm()
{
	//set all visited to false
	for (int i = 0; i < m_openList.size(); i++)
	{
		m_openList[i]->visited = false;
	}

	std::list<GridCell*> queue;
	for (auto & cell : m_openList)
	{
		if (cell->isGoal)
		{
			queue.push_back(cell);
			cell->G = 0;
			cell->visited = true;
		}
	}

	GridCell* currentCell =  queue.front();

	while (!queue.empty())
	{
		//dequeue cell from queue
		queue.pop_front();

		//get all adjecant cells of the dequeued cell. If a adjecant cell was not visiter,
		//mark it as visited and queue it.
		for (int i = 0; i < currentCell->adjecancySet.size() ; i++)
		{
			if (!currentCell->adjecancySet.at(i)->visited)
			{
				currentCell->adjecancySet.at(i)->visited = true;
				currentCell->adjecancySet.at(i)->G = currentCell->G + 1; //previous dist + 1
				queue.push_back(currentCell->adjecancySet.at(i));
				std::cout << currentCell->G + 1 << std::endl;
			}
		}
		if (!queue.empty())
		{
			currentCell = queue.front();
		}
	}
	int i = 1000;
}

void PathFinding::createAdjecancySets()
{
	int max_grid_size = 735;
	int up = -50;
	int top_right = -49;
	int right = 1;
	int bottom_right = 51;
	int bottom = 50;
	int bottom_left = 49;
	int left = -1;
	int top_left = -51;
	for (int i = 0; i < m_openList.size(); i++)
	{
		auto currentCell = m_openList[i];
		currentCell->adjecancySet.clear();
		int x = currentCell->m_xCoord;
		int y = currentCell->m_yCoord;


		if (x < max_grid_size) //if x less than last square next one is neighbor RIGHT
		{
			if (!m_openList[i + right]->blocked)
			{
				currentCell->adjecancySet.push_back(m_openList[i + right]);
			}
		}
		if (x > 0) // if x more than zero previous is neighbor LEFT
		{
			if (!m_openList[i + left]->blocked)
			{
				currentCell->adjecancySet.push_back(m_openList[i + left]);
			}
		}
		if (y > 0) // if y greater than 0 TOP
		{
			if (!m_openList[i + up]->blocked)
			{
				currentCell->adjecancySet.push_back(m_openList[i + up]);
			}
		}
		if (y < max_grid_size) // if y less tham max height BOTTOM
		{
			if (!m_openList[i + bottom]->blocked)
			{
				currentCell->adjecancySet.push_back(m_openList[i + bottom]);
			}
		}
		if (x < max_grid_size && y > 0) // TOP RIGHT
		{
			if (!m_openList[i + top_right]->blocked)
			{
				currentCell->adjecancySet.push_back(m_openList[i + top_right]);
			}
		}
		if (y < max_grid_size && x < max_grid_size) // BOTTOM RIGHT
		{
			if (!m_openList[i + bottom_right]->blocked)
			{
				currentCell->adjecancySet.push_back(m_openList[i + bottom_right]);
			}
		}
		if (y < max_grid_size && x > 0) //BOTTOM LEFT
		{
			if (!m_openList[i + bottom_left]->blocked)
			{
				currentCell->adjecancySet.push_back(m_openList[i + bottom_left]);
			}
		}
		if (y > 0 && x > 0) // TOP LEFT
		{
			if (!m_openList[i + top_left]->blocked)
			{
				currentCell->adjecancySet.push_back(m_openList[i + top_left]);
			}
		}
	}
	int i = 1000;
}

void PathFinding::SetStartAndGoal(GridCell start, GridCell goal)
{
	m_startCell = new GridCell(start.m_xCoord, start.m_yCoord, NULL);
	m_goalCell = new GridCell(goal.m_xCoord, goal.m_yCoord, &goal);

	m_startCell->G = 0;
	m_startCell->H = m_startCell->ManhattanDist(m_goalCell);
	m_startCell->parent = 0;

	m_openList.push_back(m_startCell);
}

void PathFinding::PathOpened(int x, int y, float newCost, GridCell * parent)
{
	//check if wall
	//if ()
	//{
	//	return;
	//}
	int id = y * WORLD_SIZE + x;
	for (int i = 0; id < m_visitedList.size(); i++)
	{
		if (id == m_visitedList[i]->m_id)
		{
			return;
		}
	}

	GridCell* newChild = new GridCell(x, y, parent);
	newChild->G = newCost;
	newChild->H = parent->ManhattanDist(m_goalCell);

	for (int i = 0; i < m_openList.size(); i++) 
	{
		if (id == m_openList[i]->m_id)
		{
			float newF = newChild->G + newCost + m_openList[i]->H;

			if (m_openList[i]->GetF() > newF)
			{
				m_openList[i]->G = newChild->G + newCost;
				m_openList[i]->parent = newChild;
			}
			else //if new F is not better
			{
				delete newChild;
				return;
			}
		}
	}
	m_openList.push_back(newChild);
}

GridCell * PathFinding::GetNextCell()
{
	float bestF = 999999.0f;
	int cellIndex = -1;
	GridCell* nextCell = NULL;
	
	for (int i = 0; i < m_openList.size(); i++)
	{
		if (m_openList[i]->GetF() < bestF)
		{
			bestF = m_openList[i]->GetF();
			cellIndex = i;
		}
	}

	if (cellIndex >= 0)
	{
		nextCell = m_openList[cellIndex];
		m_visitedList.push_back(nextCell);
		m_openList.erase(m_openList.begin() + cellIndex);
	}

	return nextCell;
}

void PathFinding::ContinuePath()
{
	for (int i = 0; i < 4; i++)
	{
		if (m_openList.empty())
		{
			return;
		}

		GridCell* currentCell = GetNextCell();
		if (currentCell->m_id == m_goalCell->m_id)
		{
			m_goalCell->parent = currentCell->parent;
			GridCell* getPath;

			for (getPath = m_goalCell; getPath != NULL; getPath = getPath->parent)
			{
				m_pathToGoal.push_back(new Vector2D(getPath->m_xCoord, getPath->m_yCoord));
			}

			m_foundGoal = true;
			return;
		}
		else
		{
			//right cell
			PathOpened(currentCell->m_xCoord + 1, currentCell->m_yCoord, currentCell->G + 1, currentCell);
			//left cell
			PathOpened(currentCell->m_xCoord - 1, currentCell->m_yCoord, currentCell->G + 1, currentCell);
			//top cell
			PathOpened(currentCell->m_xCoord, currentCell->m_yCoord + 1, currentCell->G + 1, currentCell);
			//bottom cell
			PathOpened(currentCell->m_xCoord, currentCell->m_yCoord - 1, currentCell->G + 1, currentCell);
			//topLeft diagonal
			PathOpened(currentCell->m_xCoord - 1, currentCell->m_yCoord + 1, currentCell->G + 1.414f, currentCell);
			//topRight diagonal
			PathOpened(currentCell->m_xCoord + 1, currentCell->m_yCoord + 1, currentCell->G + 1.414f, currentCell);
			//bottomLeft diagonal
			PathOpened(currentCell->m_xCoord - 1, currentCell->m_yCoord - 1, currentCell->G + 1.414f, currentCell);
			//bottomRight diagonal
			PathOpened(currentCell->m_xCoord + 1, currentCell->m_yCoord - 1, currentCell->G + 1.414f, currentCell);

			for (int i = 0; i < m_openList.size(); i++)
			{
				if (currentCell->m_id == m_openList[i]->m_id)
				{
					m_openList.erase(m_openList.begin() + i);
				}
			}
		}

	}
	
}
