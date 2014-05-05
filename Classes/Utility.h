#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <cmath>
#include <vector>
#include <list>

#include "cocos2d.h"
#include "BubbleConstant.h"

using namespace cocos2d;

inline CCPoint getPosByRowAndCol(int row, int col);
inline RowCol GetRowColByPos( int nPosX, int nPosY);

inline bool IsValidPos( int nRow, int nCol );
inline bool IsValidPos(RowCol nRownCol);
inline void GetAround( int nRow, int nCol, std::vector<RowCol> & vecRowCol );

//�õ�����б������ļ��ϣ���Ϊ����б��֮�֣���б�ߴ�������Ͻǵ����½ǣ� ��б�ߴ�������Ͻǵ����½�,��modeֵ������//
inline void GetGoldenLine (int nRow, int nCol, ROWCOL_LIST& listRowCol);
inline void GetSliveryLine(int nRow, int nCol, ROWCOL_LIST& listRowCol);

CCPoint getPosByRowAndCol(int row, int col)
{
	float posX, posY;

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	posX = col * 2 * BUBBLE_RADIUS + BUBBLE_RADIUS + (row % 2) * BUBBLE_RADIUS;
	posY = size.height - ( row * 2 * BUBBLE_RADIUS * sin(PI/3) + BUBBLE_RADIUS );

	return ccp(posX, posY);
}

RowCol GetRowColByPos( int nPosX, int nPosY)
{
	int nRow, nCol;
	//��Ҫ��������//

	nPosY = CCDirector::sharedDirector()->getWinSize().height - nPosY;

	nRow = ( nPosY -BUBBLE_RADIUS )/( 2 *BUBBLE_RADIUS *sin ( PI/3 ) ) +0.5;

	nCol = ( nPosX - ( nRow % 2 ) * BUBBLE_RADIUS - BUBBLE_RADIUS ) * 1.0 /(  2 *BUBBLE_RADIUS ) + 0.5;

	return RowCol( nRow, nCol );
}

//@�����Χͣ��λ�õ��б�nRow,nColΪҪ�����ͣ��λ�ã�vecPos��������Χ��λ��//
void GetAround( int nRow, int nCol, std::vector<RowCol> & vecPos )
{
	if (!IsValidPos(nRow, nCol))
	{
		return;
	}

	//ͬһ��//
	if (IsValidPos(nRow, nCol - 1))
	{
		vecPos.push_back(RowCol(nRow, nCol - 1));
	}
	if (IsValidPos(nRow, nCol + 1))
	{
		vecPos.push_back(RowCol(nRow, nCol + 1));
	}

	//��б��//
	if (IsValidPos(nRow - 1, nCol))
	{
		vecPos.push_back(RowCol(nRow - 1, nCol));
	}
	if (IsValidPos(nRow + 1, nCol))
	{
		vecPos.push_back(RowCol(nRow + 1, nCol));
	}

	//����ǵ���//
	int curCol;
	if (nRow % 2 == 0)
		curCol = nCol - 1;
	else
		curCol = nCol + 1;

	//��б��//
	if (IsValidPos(nRow - 1, curCol))
	{
		vecPos.push_back(RowCol(nRow - 1, curCol));
	}
	if (IsValidPos(nRow + 1, curCol))
	{
		vecPos.push_back(RowCol(nRow + 1, curCol));
	}

}

void GetGoldenLine(int nRow, int nCol, ROWCOL_LIST& listRowCol)
{		
	if (!IsValidPos(nRow, nCol))
		return;

	RowCol nextRowCol(nRow,nCol);
	bool nextFlag = false;
	
	listRowCol.push_back(nextRowCol);
	
	//���Ͻ�//
	do 
	{
		if (nextRowCol.m_nRow % 2 == 0)
		{
			nextRowCol.m_nRow -= 1;
			nextRowCol.m_nCol -= 1;
		}
		else
		{
			nextRowCol.m_nRow -= 1;
		}

		if (IsValidPos(nextRowCol))
		{
			listRowCol.push_back(nextRowCol);
			nextFlag = true;
		}
		else
		{
			nextFlag = false;
		}
	} while (nextFlag);

// 	���½�//
	nextRowCol.m_nRow = nRow;
	nextRowCol.m_nCol = nCol;
	do 
	{
		if (nextRowCol.m_nRow % 2 == 0)
		{
			nextRowCol.m_nRow += 1;
		}
		else
		{
			nextRowCol.m_nRow += 1;
			nextRowCol.m_nCol += 1;
		}
	
		if (IsValidPos(nextRowCol))
		{
			listRowCol.push_back(nextRowCol);
			nextFlag = true;
		}
		else
		{
			nextFlag = false;
		}
	} while (nextFlag);

	//ͬһ��//
	nextRowCol.m_nRow = nRow;
	nextRowCol.m_nCol = -1;
	do 
	{
		nextRowCol.m_nCol += 1;
	
		if (IsValidPos(nextRowCol))
		{
			listRowCol.push_back(nextRowCol);
			nextFlag = true;
		}
		else
		{
			nextFlag = false;
		}
	} while (nextFlag);
// 	
	//���Ͻ�//
	nextRowCol.m_nRow = nRow;
	nextRowCol.m_nCol = nCol;
	do 
	{
		if (nextRowCol.m_nRow % 2 == 0)
		{
			nextRowCol.m_nRow -= 1;
		}
		else
		{
			nextRowCol.m_nRow -= 1;
			nextRowCol.m_nCol += 1;
		}
	
		if (IsValidPos(nextRowCol))
		{
			listRowCol.push_back(nextRowCol);
			nextFlag = true;
		}
		else
		{
			nextFlag = false;
		}
	} while (nextFlag);
	
	//���½�//
	nextRowCol.m_nRow = nRow;
	nextRowCol.m_nCol = nCol;
	do 
	{
		if (nextRowCol.m_nRow % 2 == 0)
		{
			nextRowCol.m_nRow += 1;
			nextRowCol.m_nCol -= 1;
		}
		else
		{
			nextRowCol.m_nRow += 1;
		}
	
		if (IsValidPos(nextRowCol))
		{
			listRowCol.push_back(nextRowCol);
			nextFlag = true;
		}
		else
		{
			nextFlag = false;
		}
	} while (nextFlag);

}

void GetSliveryLine(int nRow, int nCol, ROWCOL_LIST& listRowCol)
{
	if (!IsValidPos(nRow, nCol))
		return;

	RowCol nextRowCol(nRow,nCol);
	bool nextFlag = false;

	listRowCol.push_back(nextRowCol);

	//���Ͻ�//
	do 
	{
		if (nextRowCol.m_nRow % 2 == 0)
		{
			nextRowCol.m_nRow -= 1;
			nextRowCol.m_nCol -= 1;
		}
		else
		{
			nextRowCol.m_nRow -= 1;
		}

		if (IsValidPos(nextRowCol))
		{
			listRowCol.push_back(nextRowCol);
			nextFlag = true;
		}
		else
		{
			nextFlag = false;
		}
	} while (nextFlag);

	//ͬһ��//
	nextRowCol.m_nRow = nRow;
	nextRowCol.m_nCol = -1;
	do 
	{
		nextRowCol.m_nCol += 1;

		if (IsValidPos(nextRowCol))
		{
			listRowCol.push_back(nextRowCol);
			nextFlag = true;
		}
		else
		{
			nextFlag = false;
		}
	} while (nextFlag);

	//���Ͻ�//
	nextRowCol.m_nRow = nRow;
	nextRowCol.m_nCol = nCol;
	do 
	{
		if (nextRowCol.m_nRow % 2 == 0)
		{
			nextRowCol.m_nRow -= 1;
		}
		else
		{
			nextRowCol.m_nRow -= 1;
			nextRowCol.m_nCol += 1;
		}

		if (IsValidPos(nextRowCol))
		{
			listRowCol.push_back(nextRowCol);
			nextFlag = true;
		}
		else
		{
			nextFlag = false;
		}
	} while (nextFlag);
}

//@һ��λ���Ƿ���Ч��ͣ����//
bool IsValidPos( int nRow, int nCol )
{
	if (nRow < 0 || nCol < 0)
	{
		return false;
	}
	if (nRow >= MAX_ROWS || nCol >= MAX_COLS - nRow % 2)
	{
		return false;
	}

	return true;
}

bool IsValidPos(RowCol nRownCol)
{
	return IsValidPos(nRownCol.m_nRow, nRownCol.m_nCol);
}


#endif	//_UTILITY_H_