#include "GameScene.h"
#include "Utility.h"

#include <algorithm>

GameScene::GameScene():
	m_state(GS_START),
	m_nGoldenBubbleCount(0),
	m_nSliveryBubbleCount(0),
	m_nScore(0)
{
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("game_scene_bg.mp3",true);
	SimpleAudioEngine::sharedEngine()->playEffect("ready_go.mp3");
	srand(time(NULL));
}

GameScene::~GameScene()
{
	//clear();
}

CCScene* GameScene::scene()
{
	CCScene* scene = NULL;
	do 
	{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		GameScene* layer = GameScene::create();
		CC_BREAK_IF(!layer);

		scene->addChild(layer);

	} while (0);

	return scene;
}

bool GameScene::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());

		//���ر���//
		CCSprite *background = CCSprite::create("background1.jpg");
		CC_BREAK_IF(!background);
		background->setAnchorPoint(CCPointZero);
		background->setPosition(CCPointZero);
		this->addChild(background);

		CC_BREAK_IF(!GameScene::initScheduler());
		CC_BREAK_IF(!GameScene::initBoard());
		CC_BREAK_IF(!GameScene::initReadyBubble());
		CC_BREAK_IF(!GameScene::initWaitBubble());

		this->setEnable();
		
		bRet = true;

	} while (0);

	return bRet;
}

bool GameScene::initScheduler()
{
	//this->schedule(schedule_selector(GameScene::loop), 1.0f);
	//this->scheduleUpdate();
	return true;
}

//��ʼ�����ݶ��У�//
bool GameScene::initBoard()
{
	bool bRet = false;
	do 
	{
		for (int row = 0; row < MAX_ROWS; row++)
		{
			for (int col = 0; col < MAX_COLS - row % 2; col++)
			{
				//��ʼ��ǰINIT_LINE�У� ������ΪNULL//
				if (row >= INIT_LINE)
				{
					m_board[row][col] = NULL;
					continue;
				}

				Bubble* pBubble = randomBubble();
				if (pBubble == NULL)
				{
					CC_BREAK_IF(!pBubble);
				}
				
				// 
				pBubble->setPosition(getPosByRowAndCol(row, col));
				 
				this->addChild(pBubble);
				//��������棬����list�棬Ҳû��retain����һ��ָ��//
				m_board[row][col] = pBubble;
				m_board[row][col]->setRowColIndex(row, col);
				m_listBubble.push_back(pBubble);

				bRet = true;
			}
		}
	} while (0);

	return bRet;
}

//���绯���ݷ�����//
bool GameScene::initReadyBubble()
{
	bool bRet = false;
	do 
	{
		m_curReady = randomBubble();
		CC_BREAK_IF(!m_curReady);

		CCSize size = CCDirector::sharedDirector()->getWinSize();
		m_curReady->setPosition(ccp(size.width / 2, size.height / 10));

		this->addChild(m_curReady);

		bRet = true;
	} while (0);

	return bRet;
}

bool GameScene::initWaitBubble()
{
	bool bRet = false;
	do 
	{
		for (int i = 0; i < MAX_WAIT_BUBBLE; i++)
		{
			Bubble *pBubble = randomBubble();
			CC_BREAK_IF(!pBubble);

			CCSize size = CCDirector::sharedDirector()->getWinSize();
			pBubble->setPosition(ccp(size.width/2 + (i+1) * BUBBLE_RADIUS * 2,  size.height/20));
			m_wait[i] = pBubble;
			this->addChild(pBubble);

			bRet = true;
		}
	} while (0);

	return bRet;
}
Bubble* GameScene::randomBubble()
{
	BUBBLE_COLOR color = static_cast<BUBBLE_COLOR>(rand() % (COLOR_COUNT/* - 2*/));
	Bubble *pBubble = NULL;
	do 
	{
		pBubble = Bubble::create();

		if(pBubble && pBubble->initWithFile(g_bubbleName[color].c_str()))
		{
			pBubble->setBubbleColor(color);
		}
		else
		{
			CC_SAFE_DELETE(pBubble);
		}
	} while (0);

	return pBubble;
}

Bubble* GameScene::createBubble(BUBBLE_COLOR color)
{
	Bubble *pBubble = NULL;
	do 
	{
		pBubble = Bubble::create();

		if(pBubble && pBubble->initWithFile(g_bubbleName[color].c_str()))
		{
			pBubble->setBubbleColor(color);
		}
		else
		{
			CC_SAFE_DELETE(pBubble);
		}
	} while (0);

	return pBubble;
}
//�������list�����//
void GameScene::clear()
{
	for (int nRow = 0; nRow < MAX_ROWS; nRow++)
	{
		for (int nCol = 0; nCol < MAX_COLS - nRow % 2; nCol++)
		{
			CC_SAFE_DELETE(m_board[nRow][nCol]);
		}
	}

	m_listBubble.clear();
}

bool GameScene::isCollisionWithBorder()	//�жϷ���������Ƿ�����ұ�Ե��ײ//
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCPoint pos = m_curReady->getPosition();

	if (pos.x < BUBBLE_RADIUS || pos.x > size.width - BUBBLE_RADIUS)
	{
		return true;
	}

	return false;
}

bool GameScene::isCollisionWithTopBorder(Bubble *pBubble)
{
	if (pBubble == NULL)
	{
		return false;
	}

	CCPoint pos = pBubble->getPosition();
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	if (pos.y > size.height - BUBBLE_RADIUS)
	{
		return true;
	}

	return false;
}

bool GameScene::isCollisionWithBubble(CCPoint pos1, float radius1, CCPoint pos2, float radius2)	//�Ƿ���Ϸ�������ײ����//
{
	return pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2) < pow(radius1 + radius2, 2);	//�ж���Բ�Ƿ��ཻ, ��ʽ����x1-x2)^2 + (y1-y2)^2 < (r1 + r2)^2//
}

bool GameScene::isCollision()
{
	bool bRet = false;

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	if (m_curReady->getPosition().y > size.height - BUBBLE_RADIUS)
	{
		bRet = true;
		return bRet;
	}
	//�����е���Ƚ�//
	for (BUBBLE_LIST::reverse_iterator iterBubble = m_listBubble.rbegin(); iterBubble != m_listBubble.rend(); ++iterBubble)
	{
		Bubble *pBubble = *iterBubble;
		if (pBubble && isCollisionWithBubble(pBubble->getPosition(), BUBBLE_RADIUS, m_curReady->getPosition(), BUBBLE_RADIUS))
		{
			bRet = true;
			return bRet;
		}
	}

	return bRet;
}

void GameScene::setEnable()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void GameScene::setDisableEnable()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool GameScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void GameScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{

}

void GameScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
// 	if (m_state == GS_FLY || m_state == GS_FALL)
// 	{
// 		return ;
// 	}
	m_state = GS_FLY;

	CCPoint pos = pTouch->getLocation();
	m_real = ccpNormalize(ccpSub(pos, m_curReady->getPosition()));

	setDisableEnable();//���ô�����//
	this->scheduleUpdate();
	
}

void GameScene::loop(float dt)
{

}
//�ж��߼���������//
void GameScene::update(float delta)
{
	if (isCollisionWithBorder())
	{
		m_real.x = -m_real.x;
	}

	CCPoint pos = m_curReady->getPosition();
	m_curReady->setPosition(ccp(pos.x + m_real.x * BUBBLE_SPEED, pos.y + m_real.y * BUBBLE_SPEED));

	if (isCollision())	//�����������ϱ�Ե��ײ�ˣ� ����Ӧ�Ĵ���//
	{		
		m_real = CCPointZero;
		adjustBubblePosition();

		//����ͬ����������������Ӧ��������//
		execClearBubble(m_curReady);

		//�����֮�󣬵��䴦������״̬����//
		ROWCOL_LIST fallList = checkFallBubble();
		FallBubble(fallList);

		this->unscheduleUpdate();
 		changeWaitToReady();
		setEnable();
	}

}

void GameScene::adjustBubblePosition()
{
	CCPoint curPos = m_curReady->getPosition();

	RowCol rowcol_index = GetRowColByPos(curPos.x, curPos.y);

	CCPoint adjustPos = getPosByRowAndCol(rowcol_index.m_nRow, rowcol_index.m_nCol);
	m_curReady->setPosition(adjustPos);
	m_curReady->setRowColIndex(rowcol_index.m_nRow, rowcol_index.m_nCol);

	m_board[rowcol_index.m_nRow][rowcol_index.m_nCol] = m_curReady;
	m_listBubble.push_back(m_curReady);
}

//��wait״̬���򻻳�ready״̬//
void GameScene::changeWaitToReady()
{
	m_curReady = m_wait[0];
	m_curReady->setPosition(READY_BUBBLE_POS);

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	for (int index = 0; index < MAX_WAIT_BUBBLE - 1; index++)
	{
		m_wait[index] = m_wait[index + 1];
		m_wait[index]->setPosition(ccp(size.width/2 + (index+1) * BUBBLE_RADIUS * 2, size.height/20));
	}

	m_wait[MAX_WAIT_BUBBLE - 1] = randomBubble();
	m_wait[MAX_WAIT_BUBBLE - 1]->setPosition(ccp(size.width/2+MAX_WAIT_BUBBLE * BUBBLE_RADIUS *2, size.height/20));

	this->addChild(m_wait[MAX_WAIT_BUBBLE - 1]);
}

ROWCOL_LIST GameScene::findClearBubble(Bubble *pReadyBubble)
{
	ROWCOL_LIST clearRowCollist;
	if (pReadyBubble == NULL)
	{
		return clearRowCollist;
	}
	//�����ɫ����ʲô����ģ�//
	if (pReadyBubble->getBubbleColor() == BUBBLE_GOLDEN)
	{
		clearRowCollist = findGoldenBubble(pReadyBubble);
	}
	else if (pReadyBubble->getBubbleColor() == BUBBLE_SILVERY)
	{
		clearRowCollist = findSliveryBubble(pReadyBubble);
	}
	else if (pReadyBubble->getBubbleColor() == COLOR_COUNT)//��ô�������ֵ��color��仯��//
	{
		return clearRowCollist;
	}
	else
	{
		clearRowCollist = findSameBubble(pReadyBubble);
		if (clearRowCollist.size() < REMOVE_COUNT)//3���ſ�������//
		{
			clearRowCollist.clear();
		}
	} 
	
	return clearRowCollist;
}

ROWCOL_LIST GameScene::findSameBubble(Bubble *pReadyBubble)
{
	ROWCOL_LIST samelist;
	BUBBLE_COLOR nColor= pReadyBubble->getBubbleColor();
	int nRow = pReadyBubble->getRowIndex();
	int nCol = pReadyBubble->getColumnIndex();
	samelist.push_back(RowCol(nRow, nCol));

	ROWCOL_LIST::iterator itCur = samelist.begin();

	//ѭ������ͬ������//
	do 
	{
		std::vector<RowCol> vecRowCol;

		GetAround(itCur->m_nRow, itCur->m_nCol, vecRowCol);

		for (size_t i = 0; i < vecRowCol.size(); i++)
		{
			Bubble* pCurBubble = m_board[ vecRowCol[i].m_nRow ][ vecRowCol[i].m_nCol ];
			if (pCurBubble && pCurBubble->getBubbleColor() == nColor)
			{
				RowCol rc(vecRowCol[i].m_nRow, vecRowCol[i].m_nCol);
				ROWCOL_LIST::iterator itFind = std::find(samelist.begin(), samelist.end(), rc);
				//�����Ƿ��Ѿ����б���//
				if (itFind == samelist.end())
				{
					samelist.push_back(vecRowCol[i]);
				}
			}
		}
		itCur++;
	} while (itCur != samelist.end());
	
	return samelist;
}

//���ҽ�ɫ�����������򼯺�//
ROWCOL_LIST GameScene::findGoldenBubble(Bubble *pReadyBubble)
{
	int nRow = pReadyBubble->getRowIndex();
	int nCol = pReadyBubble->getColumnIndex();
	
	ROWCOL_LIST clearGoldenList;
	GetGoldenLine(nRow, nCol, clearGoldenList);

	return clearGoldenList;
}

//������ɫ�����������򼯺�//
ROWCOL_LIST GameScene::findSliveryBubble(Bubble* pReadyBubble)
{
	int nRow = pReadyBubble->getRowIndex();
	int nCol = pReadyBubble->getColumnIndex();

	ROWCOL_LIST clearSliveryList;
	GetSliveryLine(nRow, nCol, clearSliveryList);

	return clearSliveryList;
}

/************************************************************************/
/* params: ������Ѿ�������λ�õ���
** NOTE  : ����һ���൱��Ҫ�ĺ�������Ϸ�ĺ��Ĺ��ܶ������ִ���������ݺ����У���Ҫִ�����¹���
		 1������������������ﵽGOLDEN_COUNT������ִ�н�ɫ���ݵ�Ч��
		 2��ͬ���ﵽSLIVERY_COUNT���� ��ִ����ɫ����Ч��
		 3���ﵽ3����С��SLIVERY_COUNT��������ͬ������
		 4��С��3����ʲô������
*/
/************************************************************************/
void GameScene::execClearBubble(Bubble* pReadyBubble)
{
	//Ҫִ������� ��Ҫ���ҵ���Ҫ�������//
	ROWCOL_LIST clearBubbleList = findClearBubble(pReadyBubble);
	clearBubble(clearBubbleList);
}

//����bubbleList�����е���//
void GameScene::clearBubble(const ROWCOL_LIST &bubbleList)
{
	int nRow, nCol;
	for (ROWCOL_LIST::const_iterator iterBubble = bubbleList.begin();
		iterBubble != bubbleList.end();
		iterBubble++)
	{
		nRow = iterBubble->m_nRow;
		nCol = iterBubble->m_nCol;

		Bubble *obj = m_board[nRow][nCol];
		if (obj)
		{
			removeBubbleAction(obj);
			m_board[nRow][nCol] = NULL;
		}
	
		BUBBLE_LIST::iterator itFind = std::find(m_listBubble.begin(), m_listBubble.end(), obj);
		if (itFind != m_listBubble.end())
		{
			m_listBubble.erase(itFind);
		}
		
	}
}

void GameScene::removeBubbleAction(Bubble* pBubble)
{
	pBubble->runAction(
		CCSequence::create(
			CCDelayTime::create(0.2f),
			CCFadeOut::create(0.5f),
			CCCallFuncN::create(this, callfuncN_selector(GameScene::callbackRemoveBubble)),
			NULL
		)
	);
}

void GameScene::callbackRemoveBubble(CCNode *obj)
{
	if (obj != NULL)
	{
		this->removeChild(obj, true);
		obj->autorelease();//�Ƴ���release//
	}
}

ROWCOL_LIST GameScene::checkFallBubble()
{
	ROWCOL_LIST LinkBubbleList;	//����������һ�����

	for (int i = 0; i < MAX_COLS; i++)
	{
		if (m_board[0][i] != NULL)
		{
			LinkBubbleList.push_back(RowCol(0, i));
		}
	}
	if (LinkBubbleList.empty())
	{
		return LinkBubbleList;
	}

	//���ϵ��²�����������������ʣ��û���ҵ��ľ��ǲ�������
	ROWCOL_LIST::iterator itCur = LinkBubbleList.begin();
	do 
	{
		std::vector<RowCol> vecRowCol;
		GetAround(itCur->m_nRow, itCur->m_nCol, vecRowCol);

		for (size_t i = 0; i < vecRowCol.size(); i++)
		{
			Bubble *pBubble = m_board[ vecRowCol[i].m_nRow ][ vecRowCol[i].m_nCol ];
			if (pBubble)
			{
				RowCol pos(vecRowCol[i].m_nRow, vecRowCol[i].m_nCol);
				ROWCOL_LIST::iterator itFind = std::find(LinkBubbleList.begin(), LinkBubbleList.end(), pos);

				//�����Ƿ��Ѿ����б���
				if (itFind == LinkBubbleList.end())
				{
					LinkBubbleList.push_back(vecRowCol[i]);
				}
			}
		}
		itCur++;
	} while (itCur != LinkBubbleList.end());

	ROWCOL_LIST NoLinkBubblelist;	//�ҳ�ʣ�µ�����û�����ӵ��򣬾���Ҫ�������
	for (int i = 0; i < MAX_ROWS; i++)
	{
		for (int j = 0; j < MAX_COLS - i % 2; j++)
		{
			if (m_board[i][j] != NULL)
			{
				RowCol findRowCol(i,j);
				ROWCOL_LIST::iterator itFind = std::find(LinkBubbleList.begin(), LinkBubbleList.end(), findRowCol);
				if (itFind == LinkBubbleList.end())
				{
					NoLinkBubblelist.push_back(findRowCol);
				}
			}
		}
	}

	return NoLinkBubblelist;
}

//ִ�п��Ե��������//
void GameScene::FallBubble(const ROWCOL_LIST &fallBubbleList)
{
	for (ROWCOL_LIST::const_iterator iter = fallBubbleList.begin(); iter != fallBubbleList.end(); iter++)
	{
		Bubble *pBubble = m_board[ iter->m_nRow ][ iter->m_nCol ];
		if (pBubble != NULL)
		{
			downBubbleAction(pBubble);
			
			BUBBLE_LIST::iterator iterBubble = std::find(m_listBubble.begin(), m_listBubble.end(),pBubble);
			if (iterBubble != m_listBubble.end())
			{//������+listɾ��//
				m_listBubble.erase(iterBubble);
				m_board[ iter->m_nRow ][ iter->m_nCol ] = NULL;
			}
		}
	}
	
}

//��������//
void GameScene::downBubbleAction(Bubble *pBubble)
{
	float offY = -100;

	CCPoint pos = pBubble->getPosition();
	pBubble->runAction(
			CCSequence::create(
				CCMoveTo::create((pos.y - offY) / 600.0, ccp(pos.x, offY)),
				CCCallFuncN::create(this, callfuncN_selector(GameScene::callbackRemoveBubble)),
				NULL
			)
		);
}