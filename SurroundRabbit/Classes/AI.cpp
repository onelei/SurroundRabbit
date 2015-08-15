#include "AI.h"

// 交换两个元素  ;
//   
void AI::swap(int idx1, int idx2)
{
	pAStarNode* tmp = open_table[idx1];
	open_table[idx1] = open_table[idx2];
	open_table[idx2] = tmp;
}

// 堆调整;
//   
void AI::adjust_heap(int nIndex)
{
	int curr = nIndex;
	int child = curr * 2 + 1;   // 得到左孩子idx( 下标从0开始，所有做孩子是curr*2+1 ) ; 
	int parent = (curr - 1) / 2;  // 得到双亲idx  ;

	if (nIndex < 0 || nIndex >= open_node_count)
	{
		return;
	}

	// 往下调整(要比较左右孩子和cuur parent);  
	//   
	while (child < open_node_count)
	{
		// 小根堆是双亲值小于孩子值  ;
		//   
		if (child + 1 < open_node_count && open_table[child]->s_g + open_table[child]->s_h > open_table[child + 1]->s_g + open_table[child + 1]->s_h)
		{
			++child;// 判断左右孩子大小  ;
		}

		if (open_table[curr]->s_g + open_table[curr]->s_h <= open_table[child]->s_g + open_table[child]->s_h)
		{
			break;
		}
		else
		{
			swap(child, curr);            // 交换节点 ; 
			curr = child;               // 再判断当前孩子节点  ;
			child = curr * 2 + 1;           // 再判断左孩子;  
		}
	}

	if (curr != nIndex)
	{
		return;
	}

	// 往上调整( 只需要比较cuur child和parent ) ; 
	//   
	while (curr != 0)
	{
		if (open_table[curr]->s_g + open_table[curr]->s_h >= open_table[parent]->s_g + open_table[parent]->s_h)
		{
			break;
		}
		else
		{
			swap(curr, parent);
			curr = parent;
			parent = (curr - 1) / 2;
		}
	}
}

// 判断邻居点是否可以进入open表  ;
//   
void AI::insert_to_opentable(int x, int y, pAStarNode *curr_node, pAStarNode *end_node, int w)
{
	int i;

	if (map_maze[x][y]->s_style != Node_Type_BARRIER)        // 不是障碍物  
	{
		if (!map_maze[x][y]->s_is_in_closetable)   // 不在闭表中  
		{
			if (map_maze[x][y]->s_is_in_opentable) // 在open表中  
			{
				// 需要判断是否是一条更优化的路径  ;
				//   
				if (map_maze[x][y]->s_g > curr_node->s_g + w)    // 如果更优化  
				{
					map_maze[x][y]->s_g = curr_node->s_g + w;
					map_maze[x][y]->s_parent = curr_node;

					for (i = 0; i < open_node_count; ++i)
					{
						if (open_table[i]->s_x == map_maze[x][y]->s_x && open_table[i]->s_y == map_maze[x][y]->s_y)
						{
							break;
						}
					}

					adjust_heap(i);                   // 下面调整点  ;
				}
			}
			else                                    // 不在open中  ;
			{
				map_maze[x][y]->s_g = curr_node->s_g + w;
				map_maze[x][y]->s_h = abs(end_node->s_x - x) + abs(end_node->s_y - y);
				map_maze[x][y]->s_parent = curr_node;
				map_maze[x][y]->s_is_in_opentable = 1;
				open_table[open_node_count++] = (map_maze[x][y]);
			}
		}
	}
}

// 查找邻居  ;
// 对上下左右8个邻居进行查找  
//    
void AI::get_neighbors(pAStarNode* curr_node, pAStarNode* end_node)
{
	int x = curr_node->s_x;
	int y = curr_node->s_y;

	// 下面对于8个邻居进行处理！  
	//   
	if ((x + 1) >= 0 && (x + 1) < 9 && y >= 0 && y < 9)
	{
		insert_to_opentable(x + 1, y, curr_node, end_node, 9);
	}

	if ((x - 1) >= 0 && (x - 1) < 9 && y >= 0 && y < 9)
	{
		insert_to_opentable(x - 1, y, curr_node, end_node, 9);
	}

	if (x >= 0 && x < 9 && (y + 1) >= 0 && (y + 1) < 9)
	{
		insert_to_opentable(x, y + 1, curr_node, end_node, 9);
	}

	if (x >= 0 && x < 9 && (y - 1) >= 0 && (y - 1) < 9)
	{
		insert_to_opentable(x, y - 1, curr_node, end_node, 9);
	}

	if ((x + 1) >= 0 && (x + 1) < 10 && (y + 1) >= 0 && (y + 1) < 9)
	{
		insert_to_opentable(x + 1, y + 1, curr_node, end_node, 14);
	}

	if ((x + 1) >= 0 && (x + 1) < 9 && (y - 1) >= 0 && (y - 1) < 9)
	{
		insert_to_opentable(x + 1, y - 1, curr_node, end_node, 14);
	}

	if ((x - 1) >= 0 && (x - 1) < 9 && (y + 1) >= 0 && (y + 1) < 9)
	{
		insert_to_opentable(x - 1, y + 1, curr_node, end_node, 14);
	}

	if ((x - 1) >= 0 && (x - 1) < 9 && (y - 1) >= 0 && (y - 1) < 9)
	{
		insert_to_opentable(x - 1, y - 1, curr_node, end_node, 14);
	}
}


bool AI::aStar_oneEnd(pAStarNode *_start_node, int end_zuobiao)
{
	onClear();
	int endY = end_zuobiao % 10;
	int endX = (end_zuobiao - endY)/10;
	pAStarNode *tmpStart_node = _start_node;
	pAStarNode *curr_node;
	pAStarNode *end_node = map_maze[endX][endY];

	// 下面使用A*算法得到路径  
	//    
	// 起始点加入open表 ;
	open_table[open_node_count++] = tmpStart_node;
	// 加入open表  ;
	tmpStart_node->s_is_in_opentable = 1;
	tmpStart_node->s_g = 0;
	tmpStart_node->s_h = abs(end_node->s_x - tmpStart_node->s_x) + abs(end_node->s_y - tmpStart_node->s_y);
	tmpStart_node->s_parent = NULL;

	if (tmpStart_node->s_x == end_node->s_x && tmpStart_node->s_y == end_node->s_y)
	{
		//printf("起点==终点！\n");
		return false;
	}
	// 是否找到路径 ; 
	bool is_found = false;

	while (1)
	{
		// open表的第一个点一定是f值最小的点(通过堆排序得到的);
		curr_node = open_table[0];
		// 最后一个点放到第一个点，然后进行堆调整; 
		open_table[0] = open_table[--open_node_count];
		// 调整堆 ; 
		adjust_heap(0);
		// 当前点加入close表;
		close_table[close_node_count++] = curr_node;
		// 已经在close表中了; 
		curr_node->s_is_in_closetable = 1;
		// 终点在close中，结束 ;
		if (curr_node->s_x == end_node->s_x && curr_node->s_y == end_node->s_y)
		{
			is_found = true;
			break;
		}

		// 对邻居的处理;
		get_neighbors(curr_node, end_node);

		// 没有路径到达;  
		if (open_node_count == 0)
		{
			is_found = false;
			break;
		}
	}

	if (is_found)
	{
		curr_node = end_node;
		while (curr_node)
		{
			path_stack[++top] = curr_node;
			curr_node = curr_node->s_parent;
		}
		// 输出路径;
		while (top >= 0)
		{
			int tmp = top - 1;
			if (tmp >= 0)
			{
				int _zuobiaoX = path_stack[tmp]->s_x;
				int _zuobiaoY = path_stack[tmp]->s_y;
				// 地图的开始节点变了;
				m_pStart_node = map_maze[_zuobiaoX][_zuobiaoY];
				return true;
			}
		}

	}
	return false;
}

bool AI::aStar_MultEnd(pAStarNode *_start_node, int end_zuobiao)
{
	onClear();
	int endY = end_zuobiao % 10;
	int endX = (end_zuobiao - endY) / 10;
	pAStarNode *tmpStart_node = _start_node;
	pAStarNode *curr_node;
	pAStarNode *end_node = map_maze[endX][endY];

	// 下面使用A*算法得到路径  
	//    
	// 起始点加入open表 ;
	open_table[open_node_count++] = tmpStart_node;
	// 加入open表  ;
	tmpStart_node->s_is_in_opentable = 1;
	tmpStart_node->s_g = 0;
	tmpStart_node->s_h = abs(end_node->s_x - tmpStart_node->s_x) + abs(end_node->s_y - tmpStart_node->s_y);
	tmpStart_node->s_parent = NULL;

	if (tmpStart_node->s_x == end_node->s_x && tmpStart_node->s_y == end_node->s_y)
	{
		//printf("起点==终点！\n");
		return false;
	}
	// 是否找到路径 ; 
	bool is_found = false;

	while (1)
	{
		// open表的第一个点一定是f值最小的点(通过堆排序得到的);
		curr_node = open_table[0];
		// 最后一个点放到第一个点，然后进行堆调整; 
		open_table[0] = open_table[--open_node_count];
		// 调整堆 ; 
		adjust_heap(0);
		// 当前点加入close表;
		close_table[close_node_count++] = curr_node;
		// 已经在close表中了; 
		curr_node->s_is_in_closetable = 1;
		// 终点在close中，结束 ;
		if (curr_node->s_x == end_node->s_x && curr_node->s_y == end_node->s_y)
		{
			is_found = true;
			break;
		}

		// 对邻居的处理;
		get_neighbors(curr_node, end_node);

		// 没有路径到达;  
		if (open_node_count == 0)
		{
			is_found = false;
			break;
		}
	}

	if (is_found)
	{
		curr_node = end_node;
		while (curr_node)
		{
			path_stack[++top] = curr_node;
			curr_node = curr_node->s_parent;
		}
		// 输出路径;
		while (top >= 0)
		{
			int tmp = top - 1;
			if (tmp >= 0)
			{
				int _zuobiaoX = path_stack[tmp]->s_x;
				int _zuobiaoY = path_stack[tmp]->s_y;
				// 地图的开始节点变了;
				m_pNextMove_node = map_maze[_zuobiaoX][_zuobiaoY];
				nextMoveStep = top;
				return true;
			}
		}

	}
	return false;
}

void AI::onClear()
{
	top = -1;
	open_node_count = 0;
	close_node_count = 0;
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			map_maze[i][j]->s_is_in_closetable = 0;
			map_maze[i][j]->s_is_in_opentable = 0;
			map_maze[i][j]->s_parent = NULL;
		}
	}
	for (int i = 0; i != 100; i++)
	{
		open_table[i] = nullptr;
		open_table[i] = nullptr;
		path_stack[i] = nullptr;
	}
}

void AI::addStone(int stoneTag)
{
	int y = stoneTag % 10;
	int x = (stoneTag - y) / 10;
	map_maze[x][y]->s_style = Node_Type_BARRIER;
}

bool AI::getBestAstar()
{
	int min_step = 100000;
	pAStarNode* _tmp_nextNode = nullptr;
	pAStarNode* _tmp_endNode = nullptr;

	// 找出每行最少的步数;
	for (int i = 0; i < 9;++i)
	{
		int x = map_maze[i][0]->s_x;
		int y = map_maze[i][0]->s_y;
		int end_zuobiao = x*10+y;
		if (aStar_MultEnd(m_pStart_node, end_zuobiao))
		{
			int step = nextMoveStep;
			if (step < min_step)
			{
				min_step = step;
				_tmp_nextNode = m_pNextMove_node;
				_tmp_endNode = map_maze[x][y];
			}
		}		
	}

	// 找出每列最少的步数;
	for (int i = 0; i < 9; ++i)
	{
		int x = map_maze[i][0]->s_x;
		int y = map_maze[i][0]->s_y;
		int end_zuobiao = x * 10 + y;
		if (aStar_MultEnd(m_pStart_node, end_zuobiao))
		{
			int step = nextMoveStep;
			if (step < min_step)
			{
				min_step = step;
				_tmp_nextNode = m_pNextMove_node;
				_tmp_endNode = map_maze[x][y];
			}
		}
	}

	// 找到了,返回下一点;
	if (min_step != 100000)
	{
		m_pStart_node = _tmp_nextNode;
		m_pEnd_node = _tmp_endNode;
		return true;
	}

	return false;
}

bool AI::fleeSuccess()
{
	if (m_pStart_node->s_x == m_pEnd_node->s_x&&m_pStart_node->s_y==m_pEnd_node->s_y)
	{
		return true;
	}
	return false;
}