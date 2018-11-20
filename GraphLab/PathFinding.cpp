#include "PathFinding.h"



PathFinding::PathFinding()
{
	m_initlizedStartGoal = false;
	m_foundGoal = false;

}


PathFinding::~PathFinding()
{
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
/// it will also generate the path from start cell to goal cell
/// </summary>
void PathFinding::wavefrontAlgorithm()
{
	//set all visited to false
	for (int i = 0; i < m_openList.size(); i++)
	{
		m_openList[i]->visited = false;
		m_openList[i]->parent = nullptr;
	}

	std::list<GridCell*> queue;

	queue.push_back(m_goalCell);
	m_goalCell->G = 0;
	m_goalCell->visited = true;


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
			}
		}
		if (!queue.empty())
		{
			currentCell = queue.front();
		}
	}

	Vector2D goalPos = Vector2D(m_goalCell->m_xCoord, m_goalCell->m_yCoord);
	for (auto & cell : m_openList)
	{
		if (cell->isGoal)
		{
			cell->H = 0;
		}
		Vector2D currentPos = Vector2D(cell->m_xCoord, cell->m_yCoord);
		Vector2D vecToGoal = goalPos - currentPos;
		//THIS WAS A PAIN IN THE ASS, SOMETIMES ALGORITHM WOULD GO BACK ON ITSELF BECAUSE MOVING G +1 WAS NOT AS SIGNIFICANT AS MOVING A CELL OF 15 BACK IN HEURISTIC 
		//NOW DIVIDING LENGTH BY MORE THAN SIZE OF CELL ( COULD BY MAKING JUMPS TO CELLS BIGGER AS WELL (eg. G + 20)
		cell->H = vecToGoal.Length() / 20;
	}
	GridCell* childCell;
	
	for (childCell = m_startCell; !childCell->isGoal; childCell = childCell->parent)
	{
		for (int i = 0; i < childCell->adjecancySet.size(); i++)
		{
			if (childCell->parent == nullptr || childCell->parent->GetF() > childCell->adjecancySet[i]->GetF())
			{
				childCell->parent = childCell->adjecancySet[i];
				//std::cout << "Cell : (" << childCell->m_xCoord / 15 << " ," << childCell->m_yCoord / 15 << ") " << "Changing parent to cell (" << childCell->adjecancySet.at(i)->m_xCoord / 15 << " ," << childCell->adjecancySet.at(i)->m_yCoord / 15 << ")" << std::endl;

			}
		}
	}
	for (int i = 0; i < m_pathToGoal.size(); i++)
	{
		delete m_pathToGoal[i];
	}
	m_pathToGoal.clear();
	for (auto i = m_startCell; nullptr == i || !i->isGoal;)
	{
		if (nullptr != i)
		{
			m_pathToGoal.push_back(new Vector2D(i->m_xCoord + m_cellOffset, i->m_yCoord + m_cellOffset));
			i = i->parent;
		}
		else
		{
			break;
		}
	}
	m_pathToGoal.push_back(new Vector2D(m_goalCell->m_xCoord + m_cellOffset, m_goalCell->m_yCoord + m_cellOffset));

	

}

/// <summary>
/// Creates the list of cells that are neighboring while ignoring blocked cells.
/// 
/// 
/// </summary>
void PathFinding::createAdjecancySets()
{

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
}
