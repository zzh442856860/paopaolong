/************************************************************************/
/* 
**  Date   :   2013/9/21

** author  :   Bright Moon

** function:   ��Ϸ�����߼���ȫ�����ڴ�ģ���У� ��Ҫ������
			   1����Ϸ���ݺ͵ȴ���������ݳ�ʼ��
			   2����Ϸ�����Ŀ��غʹ���
			   3�����ݵ���ײ���ʹ����������ݵ�λ�ã�
			   4����ײ�������ͬ���ݵ��㷨
			   5�����ݵ�����
			   6���������Ƿ����������ݵļ��
*/
/************************************************************************/

#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#include "Bubble.h"
#include "BubbleConstant.h"

#include <list>
#include <vector>

using namespace cocos2d;
using namespace CocosDenshion;

typedef std::list<Bubble*> BUBBLE_LIST;
typedef std::list<RowCol>  ROWCOL_LIST;

//��Ϸ�������� ���е��߼��ͱ��ֶ���������������//
class GameScene : public cocos2d::CCLayer
{
public:
	GameScene();
	~GameScene();

public:
	//������ʾ//
	static cocos2d::CCScene* scene();

	virtual bool init();

	void loop(float);
	void update(float);

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	CREATE_FUNC(GameScene);

private:
	Bubble* randomBubble();
	Bubble* createBubble(BUBBLE_COLOR color);

	void clear();

	bool initScheduler();
	bool initReadyBubble();
	bool initBoard();
	bool initWaitBubble();

	bool isCollisionWithBorder();	//�Ƿ�����ұ�Ե��ײ//

	bool isCollisionWithTopBorder(Bubble *pBubble);	//�Ƿ�Ͷ����Ե��ײ��//
	bool isCollisionWithBubble(CCPoint pos1, float radius1, CCPoint pos2, float radius2);	//�Ƿ���Ϸ�������ײ//
	bool isCollision();	//�Ƿ���ײ���������Ƿ���Ϸ����е�����ײ�Ͷ����Ե��ײ�� �����������ұ�Ե��ײ//

	void adjustBubblePosition();

	//������Ҫ�������ļ���//
	ROWCOL_LIST findClearBubble(Bubble *pReadyBubble);
	//�ҵ�ͬ�����򣬲������ҵ���ͬ������ļ���//
	ROWCOL_LIST findSameBubble(Bubble *pReadyBubble);
	//���ҽ���ɫ������Ӧ��Ҫ��������ļ���//
	ROWCOL_LIST findGoldenBubble(Bubble *pReadyBubble);
	ROWCOL_LIST findSliveryBubble(Bubble* pReadyBubble);

	//���ݵ�����������λ�ú���ɫ���ͣ� ������Ӧ�Ĵ����磺����ɫ�������ݵı�ը����������//
	void execClearBubble(Bubble* pReadyBubble);

	//�����ļ���//
	void clearBubble(const ROWCOL_LIST &bubbleList);

	//���һ����ı��ֶ���//
	void removeBubbleAction(Bubble* pBubble);
	void callbackRemoveBubble(CCNode *obj);

	ROWCOL_LIST checkFallBubble();
	//ִ�п��Ե��������//
	void FallBubble(const ROWCOL_LIST &fallBubbleList);
	//��������//
	void downBubbleAction(Bubble *pBubble);

	//��wait״̬���򻻳�ready״̬//
	void changeWaitToReady();

	void setEnable();
	void setDisableEnable();

	inline bool hasBall(int row, int col)
	{
		return m_board[row][col] != NULL;
	}

private:
	Bubble* m_board[MAX_ROWS][MAX_COLS];		//������������ܷŵ�����//

	Bubble* m_wait[MAX_WAIT_BUBBLE];			//���ڵȴ���������ݶ���//

	Bubble* m_curReady;							//��ǰ��Ҫ���������//

	BUBBLE_LIST m_listBubble;		//��ǰͼ�����е���//

	double m_dRadian;						//��ΧΪ30��Ϊ150;Ĭ��Ϊ90�ȣ� ��ֱ����//
	int m_nRotation;						//�������Ƕȸı䷽��Ҳ����ķ����Ƕ�//

	int m_nWidth;			//��Ϸ����Ŀ�Ⱥ͸߶�//
	int m_nHeight;

	int m_nScore;			//��Ϸ���õ��ķ���//

	int m_nGoldenBubbleCount;
	int m_nSliveryBubbleCount;

	GameState m_state;

	CCPoint m_real;

};
#endif