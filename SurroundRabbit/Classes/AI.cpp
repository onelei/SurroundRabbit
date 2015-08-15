#include "AI.h"

// ��������Ԫ��  ;
//   
void AI::swap(int idx1, int idx2)
{
	pAStarNode* tmp = open_table[idx1];
	open_table[idx1] = open_table[idx2];
	open_table[idx2] = tmp;
}

// �ѵ���;
//   
void AI::adjust_heap(int nIndex)
{
	int curr = nIndex;
	int child = curr * 2 + 1;   // �õ�����idx( �±��0��ʼ��������������curr*2+1 ) ; 
	int parent = (curr - 1) / 2;  // �õ�˫��idx  ;

	if (nIndex < 0 || nIndex >= open_node_count)
	{
		return;
	}

	// ���µ���(Ҫ�Ƚ����Һ��Ӻ�cuur parent);  
	//   
	while (child < open_node_count)
	{
		// С������˫��ֵС�ں���ֵ  ;
		//   
		if (child + 1 < open_node_count && open_table[child]->s_g + open_table[child]->s_h > open_table[child + 1]->s_g + open_table[child + 1]->s_h)
		{
			++child;// �ж����Һ��Ӵ�С  ;
		}

		if (open_table[curr]->s_g + open_table[curr]->s_h <= open_table[child]->s_g + open_table[child]->s_h)
		{
			break;
		}
		else
		{
			swap(child, curr);            // �����ڵ� ; 
			curr = child;               // ���жϵ�ǰ���ӽڵ�  ;
			child = curr * 2 + 1;           // ���ж�����;  
		}
	}

	if (curr != nIndex)
	{
		return;
	}

	// ���ϵ���( ֻ��Ҫ�Ƚ�cuur child��parent ) ; 
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

// �ж��ھӵ��Ƿ���Խ���open��  ;
//   
void AI::insert_to_opentable(int x, int y, pAStarNode *curr_node, pAStarNode *end_node, int w)
{
	int i;

	if (map_maze[x][y]->s_style != Node_Type_BARRIER)        // �����ϰ���  
	{
		if (!map_maze[x][y]->s_is_in_closetable)   // ���ڱձ���  
		{
			if (map_maze[x][y]->s_is_in_opentable) // ��open����  
			{
				// ��Ҫ�ж��Ƿ���һ�����Ż���·��  ;
				//   
				if (map_maze[x][y]->s_g > curr_node->s_g + w)    // ������Ż�  
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

					adjust_heap(i);                   // ���������  ;
				}
			}
			else                                    // ����open��  ;
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

// �����ھ�  ;
// ����������8���ھӽ��в���  
//    
void AI::get_neighbors(pAStarNode* curr_node, pAStarNode* end_node)
{
	int x = curr_node->s_x;
	int y = curr_node->s_y;

	// �������8���ھӽ��д���  
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

	// ����ʹ��A*�㷨�õ�·��  
	//    
	// ��ʼ�����open�� ;
	open_table[open_node_count++] = tmpStart_node;
	// ����open��  ;
	tmpStart_node->s_is_in_opentable = 1;
	tmpStart_node->s_g = 0;
	tmpStart_node->s_h = abs(end_node->s_x - tmpStart_node->s_x) + abs(end_node->s_y - tmpStart_node->s_y);
	tmpStart_node->s_parent = NULL;

	if (tmpStart_node->s_x == end_node->s_x && tmpStart_node->s_y == end_node->s_y)
	{
		//printf("���==�յ㣡\n");
		return false;
	}
	// �Ƿ��ҵ�·�� ; 
	bool is_found = false;

	while (1)
	{
		// open��ĵ�һ����һ����fֵ��С�ĵ�(ͨ��������õ���);
		curr_node = open_table[0];
		// ���һ����ŵ���һ���㣬Ȼ����жѵ���; 
		open_table[0] = open_table[--open_node_count];
		// ������ ; 
		adjust_heap(0);
		// ��ǰ�����close��;
		close_table[close_node_count++] = curr_node;
		// �Ѿ���close������; 
		curr_node->s_is_in_closetable = 1;
		// �յ���close�У����� ;
		if (curr_node->s_x == end_node->s_x && curr_node->s_y == end_node->s_y)
		{
			is_found = true;
			break;
		}

		// ���ھӵĴ���;
		get_neighbors(curr_node, end_node);

		// û��·������;  
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
		// ���·��;
		while (top >= 0)
		{
			int tmp = top - 1;
			if (tmp >= 0)
			{
				int _zuobiaoX = path_stack[tmp]->s_x;
				int _zuobiaoY = path_stack[tmp]->s_y;
				// ��ͼ�Ŀ�ʼ�ڵ����;
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

	// ����ʹ��A*�㷨�õ�·��  
	//    
	// ��ʼ�����open�� ;
	open_table[open_node_count++] = tmpStart_node;
	// ����open��  ;
	tmpStart_node->s_is_in_opentable = 1;
	tmpStart_node->s_g = 0;
	tmpStart_node->s_h = abs(end_node->s_x - tmpStart_node->s_x) + abs(end_node->s_y - tmpStart_node->s_y);
	tmpStart_node->s_parent = NULL;

	if (tmpStart_node->s_x == end_node->s_x && tmpStart_node->s_y == end_node->s_y)
	{
		//printf("���==�յ㣡\n");
		return false;
	}
	// �Ƿ��ҵ�·�� ; 
	bool is_found = false;

	while (1)
	{
		// open��ĵ�һ����һ����fֵ��С�ĵ�(ͨ��������õ���);
		curr_node = open_table[0];
		// ���һ����ŵ���һ���㣬Ȼ����жѵ���; 
		open_table[0] = open_table[--open_node_count];
		// ������ ; 
		adjust_heap(0);
		// ��ǰ�����close��;
		close_table[close_node_count++] = curr_node;
		// �Ѿ���close������; 
		curr_node->s_is_in_closetable = 1;
		// �յ���close�У����� ;
		if (curr_node->s_x == end_node->s_x && curr_node->s_y == end_node->s_y)
		{
			is_found = true;
			break;
		}

		// ���ھӵĴ���;
		get_neighbors(curr_node, end_node);

		// û��·������;  
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
		// ���·��;
		while (top >= 0)
		{
			int tmp = top - 1;
			if (tmp >= 0)
			{
				int _zuobiaoX = path_stack[tmp]->s_x;
				int _zuobiaoY = path_stack[tmp]->s_y;
				// ��ͼ�Ŀ�ʼ�ڵ����;
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

	// �ҳ�ÿ�����ٵĲ���;
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

	// �ҳ�ÿ�����ٵĲ���;
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

	// �ҵ���,������һ��;
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