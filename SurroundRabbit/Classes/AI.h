#ifndef __AI__
#define __AI__

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <stdio.h>  
#include <stdlib.h>  

#define Node_Type_START   1  
#define Node_Type_END     2  
#define Node_Type_BARRIER     3  

using namespace std;

class pAStarNode
{
public:
	int tag;	// ��Զ������;
	int s_x;    // ����(�������·����Ҫ);  
	int s_y;
	int s_g;    // ��㵽�˵�ľ���( ��g��h���Եõ�f���˴�fʡ�ԣ�f=g+h );
	int s_h;    // ��������Ԥ��Ĵ˵㵽�յ�ľ���;
	int s_style;// ������ͣ���ʼ�㣬�յ㣬�ϰ���;
	class pAStarNode * s_parent;    // ���ڵ�;
	int s_is_in_closetable;     // �Ƿ���close���� ; 
	int s_is_in_opentable;      // �Ƿ���open���� ; 
};

class AI{

public:
	pAStarNode *start_node;          // ��ʼ��;  
	pAStarNode *end_node;            // ������ ; 
	pAStarNode *curr_node;           // ��ǰ��  ;
	pAStarNode* map_maze[9][9];        // �������  ;
	pAStarNode* open_table[100];     // open�� ; 
	pAStarNode* close_table[100];        // close�� ; 
	int open_node_count=0;// open���нڵ�����  ;
	int close_node_count=0;// close���н������  ;
	pAStarNode* path_stack[100];     // ����·����ջ ; 
	int top = -1; // ջ�� ; 

	void swap(int idx1, int idx2);
	void adjust_heap(int nIndex);
	void insert_to_opentable(int x, int y, pAStarNode* curr_node, pAStarNode* end_node, int w);
	void get_neighbors(pAStarNode* curr_node, pAStarNode* end_node);
	bool aStar(pAStarNode* startNode, int endTag);
	void onClear();
	void addStone(int stoneTag);
};
#endif /* defined(__AI__) */