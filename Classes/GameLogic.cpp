/*
 * GameLogic.cpp
 *
 */

#include "GameLogic.h"
BYTE GameLogic::m_cbCardListData[54] =
{
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,	//方块 A - K
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,	//梅花 A - K
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,	//红桃 A - K
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,	//黑桃 A - K
	0x4E, 0x4F
};
GameLogic::GameLogic()
{
}
GameLogic::~GameLogic() {
}
//获取牛牛
bool GameLogic::GetOxCard(BYTE cbCardData[], BYTE cbCardCount)
{
	//ASSERT(cbCardCount == MAX_COUNT);

	//设置变量
	BYTE bTemp[MAX_COUNT], bTempData[MAX_COUNT];
	CopyMemory(bTempData, cbCardData, sizeof(bTempData));
	BYTE bSum = 0;
	for (BYTE i = 0; i < cbCardCount; i++)
	{
		bTemp[i] = GetCardLogicValue(cbCardData[i]);
		bSum += bTemp[i];
	}

	//查找牛牛
	for (BYTE i = 0; i < cbCardCount - 1; i++)
	{
		for (BYTE j = i + 1; j < cbCardCount; j++)
		{
			if ((bSum - bTemp[i] - bTemp[j]) % 10 == 0)
			{
				BYTE bCount = 0;
				for (BYTE k = 0; k < cbCardCount; k++)
				{
					if (k != i && k != j)
					{
						cbCardData[bCount++] = bTempData[k];
					}
				}
				//ASSERT(bCount == 3);
				
				cbCardData[bCount++] = bTempData[i];
				cbCardData[bCount++] = bTempData[j];

				return true;
			}
		}
	}

	return false;
}
//逻辑数值
BYTE GameLogic::GetCardLogicValue(BYTE cbCardData)
{
	//扑克属性
	BYTE bCardColor = GetCardColor(cbCardData);
	BYTE bCardValue = GetCardValue(cbCardData);

	//大小王转化为10点
	if (bCardValue == 0x0E || bCardValue == 0x0F)
		bCardValue = 10;

	//转换数值
	return (bCardValue > 10) ? (10) : bCardValue;
}
//排列扑克
void GameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount)
{
	//转换数值
	BYTE cbLogicValue[MAX_COUNT];
	for (BYTE i = 0; i < cbCardCount; i++)
		cbLogicValue[i] = GetCardValue(cbCardData[i]);

	//排序操作
	bool bSorted = true;
	BYTE cbTempData, bLast = cbCardCount - 1;
	do
	{
		bSorted = true;
		for (BYTE i = 0; i < bLast; i++)
		{
			if ((cbLogicValue[i] < cbLogicValue[i + 1]) ||
				((cbLogicValue[i] == cbLogicValue[i + 1]) && (cbCardData[i] < cbCardData[i + 1])))
			{
				//交换位置
				cbTempData = cbCardData[i];
				cbCardData[i] = cbCardData[i + 1];
				cbCardData[i + 1] = cbTempData;
				cbTempData = cbLogicValue[i];
				cbLogicValue[i] = cbLogicValue[i + 1];
				cbLogicValue[i + 1] = cbTempData;
				bSorted = false;
			}
		}
		bLast--;
	} while (bSorted == false);

	return;
}
//获取类型
BYTE GameLogic::GetCardType(BYTE cbCardData[], BYTE cbCardCount)
{
	//ASSERT(cbCardCount == MAX_COUNT);

	BYTE bKingCount = 0, bTenCount = 0;
	for (BYTE i = 0; i<cbCardCount; i++)
	{
		if (GetCardValue(cbCardData[i])>10)
		{
			bKingCount++;
		}
		else if (GetCardValue(cbCardData[i]) == 10)
		{
			bTenCount++;
		}
	}

	if (bKingCount == MAX_COUNT)	//全花牌，包括大小王
		return OX_FIVEKING;
	else if (bKingCount == MAX_COUNT - 1 && bTenCount == 1) //四个花牌一个十
		return OX_FOURKING;

	BYTE bTemp[MAX_COUNT];
	BYTE bSum = 0;
	for (BYTE i = 0; i < cbCardCount; i++)
	{
		bTemp[i] = GetCardLogicValue(cbCardData[i]);
		bSum += bTemp[i];
	}

	for (BYTE i = 0; i < cbCardCount - 1; i++)
	{
		for (BYTE j = i + 1; j<cbCardCount; j++)
		{
			if ((bSum - bTemp[i] - bTemp[j]) % 10 == 0)
			{
				return ((bTemp[i] + bTemp[j])>10) ? (bTemp[i] + bTemp[j] - 10) : (bTemp[i] + bTemp[j]);
			}
		}
	}

	return OX_VALUE0;
}
//对比扑克
bool GameLogic::CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount)
{

	//获取点数
	BYTE cbNextType = GetCardType(cbNextData, cbCardCount);
	BYTE cbFirstType = GetCardType(cbFirstData, cbCardCount);

	//点数判断
	if (cbFirstType != cbNextType)
		return (cbFirstType > cbNextType);

	//排序大小
	BYTE bFirstTemp[MAX_COUNT], bNextTemp[MAX_COUNT];
	CopyMemory(bFirstTemp, cbFirstData, cbCardCount);
	CopyMemory(bNextTemp, cbNextData, cbCardCount);
	SortCardList(bFirstTemp, cbCardCount);
	SortCardList(bNextTemp, cbCardCount);

	//比较数值
	BYTE cbNextMaxValue = GetCardValue(bNextTemp[0]);
	BYTE cbFirstMaxValue = GetCardValue(bFirstTemp[0]);
	if (cbNextMaxValue != cbFirstMaxValue)return cbFirstMaxValue > cbNextMaxValue;

	//比较颜色
	return GetCardColor(bFirstTemp[0]) > GetCardColor(bNextTemp[0]);

	return false;
}
//对比扑克
bool GameLogic::CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount, bool FirstOX, bool NextOX)
{
	if (FirstOX != NextOX)return (FirstOX > NextOX);

	//比较牛大小
	if (FirstOX == true)
	{
		//获取点数
		BYTE cbNextType = GetCardType(cbNextData, cbCardCount);
		BYTE cbFirstType = GetCardType(cbFirstData, cbCardCount);

		//点数判断
		if (cbFirstType != cbNextType) return (cbFirstType > cbNextType);
	}

	//排序大小
	BYTE bFirstTemp[MAX_COUNT], bNextTemp[MAX_COUNT];
	CopyMemory(bFirstTemp, cbFirstData, cbCardCount);
	CopyMemory(bNextTemp, cbNextData, cbCardCount);
	SortCardList(bFirstTemp, cbCardCount);
	SortCardList(bNextTemp, cbCardCount);

	//比较数值
	BYTE cbNextMaxValue = GetCardValue(bNextTemp[0]);
	BYTE cbFirstMaxValue = GetCardValue(bFirstTemp[0]);
	if (cbNextMaxValue != cbFirstMaxValue)return cbFirstMaxValue > cbNextMaxValue;

	//比较颜色
	return GetCardColor(bFirstTemp[0]) > GetCardColor(bNextTemp[0]);

	return false;
}
