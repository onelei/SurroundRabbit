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
	int tag;	// 永远不会变的;
	int s_x;    // 坐标(最终输出路径需要);  
	int s_y;
	int s_g;    // 起点到此点的距离( 由g和h可以得到f，此处f省略，f=g+h );
	int s_h;    // 启发函数预测的此点到终点的距离;
	int s_style;// 结点类型：起始点，终点，障碍物;
	class pAStarNode * s_parent;    // 父节点;
	int s_is_in_closetable;     // 是否在close表中 ; 
	int s_is_in_opentable;      // 是否在open表中 ; 
};

class AI{

public:
	pAStarNode *start_node;          // 起始点;  
	pAStarNode *end_node;            // 结束点 ; 
	pAStarNode *curr_node;           // 当前点  ;
	pAStarNode* map_maze[9][9];        // 结点数组  ;
	pAStarNode* open_table[100];     // open表 ; 
	pAStarNode* close_table[100];        // close表 ; 
	int open_node_count=0;// open表中节点数量  ;
	int close_node_count=0;// close表中结点数量  ;
	pAStarNode* path_stack[100];     // 保存路径的栈 ; 
	int top = -1; // 栈顶 ; 

	void swap(int idx1, int idx2);
	void adjust_heap(int nIndex);
	void insert_to_opentable(int x, int y, pAStarNode* curr_node, pAStarNode* end_node, int w);
	void get_neighbors(pAStarNode* curr_node, pAStarNode* end_node);
	bool aStar(pAStarNode* startNode, int endTag);
	void onClear();
	void addStone(int stoneTag);
};
#endif /* defined(__AI__) */