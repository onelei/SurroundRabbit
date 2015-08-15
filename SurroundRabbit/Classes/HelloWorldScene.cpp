#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include <sstream>

USING_NS_CC;

using namespace cocostudio::timeline;

int HelloWorld::Num_Lie = 9;
int HelloWorld::Num_Hang = 9;

const int NUM_DEATALSTONE = 4;

const string STR_STEP = "Step:";
const string STR_BEST = "Best:";

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	m_pGameAI = new AI();

	auto pScene = CSLoader::createNode("MainScene.csb");

	addChild(pScene);

	
	//获取所有节点;
	m_pNodes = pScene->getChildByName("Sprite_bg");

	//分别的到兔子和石头;
	for (auto it = m_pNodes->getChildren().begin(); it != m_pNodes->getChildren().end(); it++) 
	{
		int tag = (*it)->getTag();
		// 个位;
		int j = (tag % 10) -1;
		// 十位;
		int i = ((tag - j)/10) -1;
		pAStarNode* oneNode = new pAStarNode();
		oneNode->s_g = 0;
		oneNode->s_h = 0;
		oneNode->s_is_in_closetable = 0;
		oneNode->s_is_in_opentable = 0;
		oneNode->s_style = 0;
		oneNode->s_x = i;
		oneNode->s_y = j;
		oneNode->s_parent = NULL;
		m_pGameAI->map_maze[i][j] = oneNode;
		if ((*it)->getName() == "Sprite_Rabbit") 
		{
			m_pNode_rabbit = (*it);
			int a = m_pNode_rabbit->getTag();
			m_pGameAI->start_node = m_pGameAI->map_maze[i][j];
		}
		else
		{
			m_pNode_stones.pushBack((*it));
		}
	}

	//add listener;
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBeganCallBack, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void HelloWorld::onEnter(){
	Layer::onEnter();
	// 随机石头(8个,4个);

	this->setRandStones(8 + rand_0_1() * 4);

}

void HelloWorld::onExit(){
	Layer::onExit();

}

void HelloWorld::setRandStones(int num)
{
	int curNum = 0;
	//随机产生石头;
	while (num != curNum) 
	{
		int ver, hor, tag;
		
		ver = 1 + rand_0_1() * 8;
		hor = 1 + rand_0_1() * 8;
		tag = ver*10 + hor;
		bool isvisi = m_pNodes->getChildByTag(tag)->isVisible();
		if (!isvisi&&(tag!=55))
		{
			m_pNodes->getChildByTag(tag)->setVisible(true);
			int stone_zuobiao = tag - 11;
			m_pGameAI->addStone(stone_zuobiao);
			curNum++;
		}
		else
		{
			continue;
		}

	}

}

// 触摸事件;
bool HelloWorld::onTouchBeganCallBack(Touch *t, Event* event)
{
	for (auto it = m_pNode_stones.begin(); it != m_pNode_stones.end(); it++) 
	{
		if ((*it)->getBoundingBox().containsPoint(t->getLocation()) && (!(*it)->isVisible()))
		{
			//add stone
			auto stone = (*it);
			stone->setVisible(true);
			int stone_zuobiao = stone->getTag()-11;
			m_pGameAI->addStone(stone_zuobiao);
			int end_zuobiao = 01;
			bool isSuccess = m_pGameAI->aStar(m_pGameAI->start_node, end_zuobiao);
			if (isSuccess)
			{
				int zuobiao = m_pGameAI->start_node->s_x * 10 + m_pGameAI->start_node->s_y;
				int nextTag = zuobiao + 11;
				auto nextNode = m_pNodes->getChildByTag(nextTag);
				m_pNode_rabbit->setPosition(nextNode->getPosition());
			}
			else
			{
				MessageBox("结束!!", "Stone");
			}
			int a = 0;
			return true;
		}
	}
	return false;
}



