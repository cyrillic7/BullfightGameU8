#include "GameLogicOneByOne.h"
#include "../Network/CMD_Server/CMD_Ox_OneByOne.h"
#include "../Network/CMD_Server/PacketAide.h"
//////////////////////////////////////////////////////////////////////////

//扑克数据
BYTE GameLogicOneByOne::m_cbCardListData[54]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K
	0x41,0x42
};

//////////////////////////////////////////////////////////////////////////

//构造函数
GameLogicOneByOne::GameLogicOneByOne()
{
}

//析构函数
GameLogicOneByOne::~GameLogicOneByOne()
{
}

//获取类型
//BYTE CGameLogic::GetCardType(BYTE cbCardData[], BYTE cbCardCount)
//{
//	ASSERT(cbCardCount==MAX_COUNT);
//
//	BYTE bKingCount=0,bTenCount=0;
//	for(BYTE i=0;i<cbCardCount;i++)
//	{
//		if(GetCardValue(cbCardData[i])>10)
//		{
//			bKingCount++;
//		}
//		else if(GetCardValue(cbCardData[i])==10)
//		{
//			bTenCount++;
//		}
//	}
//	BYTE bTemp[MAX_COUNT];
//	BYTE bSum=0;
//	for (BYTE i=0;i<cbCardCount;i++)
//	{
//		bTemp[i]=GetCardLogicValue(cbCardData[i]);
//		bSum+=bTemp[i];
//	}
//
//	for (BYTE i=0;i<cbCardCount-1;i++)
//	{
//		for (BYTE j=i+1;j<cbCardCount;j++)
//		{
//			if((bSum-bTemp[i]-bTemp[j])%10==0)
//			{
//				return ((bTemp[i]+bTemp[j])>10)?(bTemp[i]+bTemp[j]-10):(bTemp[i]+bTemp[j]);
//			}
//		}
//	}
//	return OX_VALUE0;
//}

//获取牌型
BYTE GameLogicOneByOne::GetCardType(const BYTE cbCardData[], BYTE cbCardCount,BYTE *bcOutCadData )
{
	//合法判断
	assert(5==cbCardCount);
	if (5!=cbCardCount) return 0;

	//排序扑克
	BYTE cbCardDataSort[MAXCOUNT];
	CopyMemory(cbCardDataSort,cbCardData,sizeof(BYTE)*cbCardCount);
	SortCardList(cbCardDataSort,cbCardCount);

	if(bcOutCadData != NULL)
	{
		CopyMemory(bcOutCadData,cbCardDataSort,cbCardCount);
	}

	int n = 0;
	BYTE bcMakeMax[5];
	memset(bcMakeMax,0,5);
	int iBigValue = 0;
	BYTE iSingleA[2];
	int iIndex = 0;
	bcMakeMax[0]= cbCardDataSort[n];

	int iGetTenCount = 0;

	for (int iten = 0;iten<cbCardCount;iten++)
	{
		if(GetCardLogicValue(cbCardDataSort[iten])==10||GetCardLogicValue(cbCardDataSort[iten])==11)
			iGetTenCount++;
	}
	if( iGetTenCount>=3)
	{
		if(GetCardColor(cbCardDataSort[0])==0x04&&GetCardColor(cbCardDataSort[1])==0x04)
		{
			if(bcOutCadData != NULL)
			{
				bcOutCadData[0] = cbCardDataSort[0];
				bcOutCadData[1] = cbCardDataSort[3];
				bcOutCadData[2] = cbCardDataSort[4];
				bcOutCadData[3] = cbCardDataSort[1];
				bcOutCadData[4] = cbCardDataSort[2];

			}
			return 10;

		}
		if(GetCardColor(cbCardDataSort[0])==0x04)
		{
			//大小王与最小的组合成牛 
			if(bcOutCadData != NULL)
			{
				bcOutCadData[0] = cbCardDataSort[0];
				bcOutCadData[1] = cbCardDataSort[3];
				bcOutCadData[2] = cbCardDataSort[4];
				bcOutCadData[3] = cbCardDataSort[1];
				bcOutCadData[4] = cbCardDataSort[2];
			}
			return 10;
		}
		else
		{
			iBigValue=GetCardLogicValue(cbCardDataSort[3])+GetCardLogicValue(cbCardDataSort[4]);
			return (iBigValue%10==0)?(10):iBigValue%10;
		}

	}
	if(iGetTenCount==2||(iGetTenCount==1&&GetCardColor(cbCardDataSort[0])==0x04))
	{

		if(GetCardColor(cbCardDataSort[0])==0x04&&GetCardColor(cbCardDataSort[1])==0x04)
		{
			if(bcOutCadData != NULL)
			{
				bcOutCadData[0] = cbCardDataSort[0];
				bcOutCadData[1] = cbCardDataSort[3];
				bcOutCadData[2] = cbCardDataSort[4];
				bcOutCadData[3] = cbCardDataSort[1];
				bcOutCadData[4] = cbCardDataSort[2];
			}
			return 10;
		}else
		{
			//如果有一张王 其他任意三张组合为10则是牛牛
			if(GetCardColor(cbCardDataSort[0])==0x04)
			{

				for ( n=1;n<cbCardCount;n++)
				{
					for (int j = 1;j<cbCardCount;j++)
					{
						if(j != n)
						{
							for (int w = 1;w<cbCardCount;w++)
							{
								if(w != n&&w!=j)
								{
									//如果剩余的四张中任意三张能组合位10的整数倍

									if((GetCardLogicValue(cbCardDataSort[n])+GetCardLogicValue(cbCardDataSort[j])+GetCardLogicValue(cbCardDataSort[w]))%10==0)
									{
										int add = 0;
										for (int y = 1;y<cbCardCount;y++)
										{
											if(y != n&&y!=j&&y!=w)
											{
												iSingleA[add] =cbCardDataSort[y]; 
												add++;
											}
										}
										if(bcOutCadData != NULL)
										{
											bcOutCadData[0] = cbCardDataSort[n];
											bcOutCadData[1] = cbCardDataSort[j];
											bcOutCadData[2] = cbCardDataSort[w];
											bcOutCadData[3] = cbCardDataSort[0];
											bcOutCadData[4] = iSingleA[0];
										}
										return 10;
									}
								}
							}
						}
					}
				}
				//如果有一张王 其他任意三张组合不为10则 取两张点数最大的组合
				BYTE bcTmp[4];
				int iBig = 0;
				int in = 0;
				for ( in = 1;in<cbCardCount;in++)
				{
					for (int j = 1;j<cbCardCount;j++)
					{
						if(in != j)
						{
							BYTE bclogic = (GetCardLogicValue(cbCardDataSort[in])+GetCardLogicValue(cbCardDataSort[j]))%10;
							if(bclogic>iBig)
							{
								iBig = bclogic;
								int add = 0;
								bcTmp[0]=cbCardDataSort[in];
								bcTmp[1]=cbCardDataSort[j];
								for (int y = 1;y<cbCardCount;y++)
								{
									if(y != in&&y!=j)
									{
										iSingleA[add] =cbCardDataSort[y]; 
										add++;
									}
								}
								bcTmp[2]=iSingleA[0];
								bcTmp[3]=iSingleA[1];
							}
						}
					}   
				}
				if(bcOutCadData != NULL)
				{
					bcOutCadData[0] = cbCardDataSort[0];
					bcOutCadData[1] = bcTmp[2];
					bcOutCadData[2] = bcTmp[3];
					bcOutCadData[3] = bcTmp[0];
					bcOutCadData[4] = bcTmp[1];
				}
				if(iGetTenCount==1&&GetCardColor(cbCardDataSort[0])==0x04)
				{
					//下面还能组合 有两张为 10 也可以组合成牛牛

				}else
				{
					//如果没有则比较 完与最小组合最大点数和组合
					iBigValue=GetCardLogicValue(bcTmp[0])+GetCardLogicValue(bcTmp[1]);
					return (iBigValue%10==0)?(10):iBigValue%10;
				}


			}else
			{
				if((GetCardLogicValue(cbCardDataSort[2])+GetCardLogicValue(cbCardDataSort[3])+GetCardLogicValue(cbCardDataSort[4]))%10==0)
				{
					if(bcOutCadData != NULL)
					{
						bcOutCadData[0] = cbCardDataSort[2];
						bcOutCadData[1] = cbCardDataSort[3];
						bcOutCadData[2] = cbCardDataSort[4];
						bcOutCadData[3] = cbCardDataSort[0];
						bcOutCadData[4] = cbCardDataSort[1];
					}
					return 10;
				}else
				{
					for ( n= 2;n<cbCardCount;n++)
					{
						for (int j = 2;j<cbCardCount;j++)
						{
							if(j != n)
							{
								if((GetCardLogicValue(cbCardDataSort[n])+GetCardLogicValue(cbCardDataSort[j]))%10==0)
								{
									int add = 0;
									for (int y = 2;y<cbCardCount;y++)
									{
										if(y != n&&y!=j)
										{
											iSingleA[add] =cbCardDataSort[y]; 
											add++;

										}
									}
									if(iBigValue<=iSingleA[0]%10)
									{
										iBigValue = GetCardLogicValue(iSingleA[0])%10;
										if(bcOutCadData != NULL)
										{
											bcOutCadData[0]= cbCardDataSort[0];
											bcOutCadData[1]= cbCardDataSort[n]; 
											bcOutCadData[2]= cbCardDataSort[j]; 
											bcOutCadData[3]= cbCardDataSort[1];
											bcOutCadData[4]= iSingleA[0]; 
										}
										if(iBigValue==0)
											return 10;
									}
								}
							}
						}
					}
					if(iBigValue != 0)
					{
						return (iBigValue%10==0)?(10):iBigValue%10;
					}
				}
			}
		}
		iGetTenCount = 1;
	}
	//4个组合
	if(iGetTenCount==1)
	{
		if(GetCardColor(cbCardDataSort[0])==0x04)
		{
			for ( n= 1;n<cbCardCount;n++)
			{
				for (int j = 1;j<cbCardCount;j++)
				{
					if(j != n)
					{
						//任意两张组合成牛
						if((GetCardLogicValue(cbCardDataSort[n])+GetCardLogicValue(cbCardDataSort[j]))%10==0)
						{
							int add = 0;
							for (int y = 1;y<cbCardCount;y++)
							{
								if(y != n&&y!=j)
								{
									iSingleA[add] =cbCardDataSort[y]; 
									add++;
								}
							}
							if(bcOutCadData != NULL)
							{
								bcOutCadData[0] = cbCardDataSort[0];
								bcOutCadData[1] = iSingleA[0];
								bcOutCadData[2] = iSingleA[1];
								bcOutCadData[3] = cbCardDataSort[n];
								bcOutCadData[4] = cbCardDataSort[j];
							}
							return 10;
						}
					}
				}
			}
			//取4张中组合最大的点数
			BYTE bcTmp[4];
			int iBig = 0;
			int in = 0;
			for ( in = 1;in<cbCardCount;in++)
			{
				for (int j = 1;j<cbCardCount;j++)
				{
					if(in != j)
					{
						BYTE bclogic = (GetCardLogicValue(cbCardDataSort[in])+GetCardLogicValue(cbCardDataSort[j]))%10;
						if(bclogic>iBig)
						{
							iBig = bclogic;
							int add = 0;
							bcTmp[0]=cbCardDataSort[in];
							bcTmp[1]=cbCardDataSort[j];
							for (int y = 1;y<cbCardCount;y++)
							{
								if(y != in&&y!=j)
								{
									iSingleA[add] =cbCardDataSort[y]; 
									add++;
								}
							}
							bcTmp[2]=iSingleA[0];
							bcTmp[3]=iSingleA[1];
						}
					}
				}   
			}
			if(bcOutCadData != NULL)
			{
				bcOutCadData[0] = cbCardDataSort[0];
				bcOutCadData[1] = bcTmp[2];
				bcOutCadData[2] = bcTmp[3];
				bcOutCadData[3] = bcTmp[0];
				bcOutCadData[4] = bcTmp[1];
			}
			iBigValue=GetCardLogicValue(bcTmp[0])+GetCardLogicValue(bcTmp[1]);
			return (iBigValue%10==0)?(10):iBigValue%10;

		}
		//取4张中任两张组合为10 然后求另外两张的组合看是否是组合中最大
		for ( n= 1;n<cbCardCount;n++)
		{
			for (int j = 1;j<cbCardCount;j++)
			{
				if(j != n)
				{
					if((GetCardLogicValue(cbCardDataSort[n])+GetCardLogicValue(cbCardDataSort[j]))%10==0)
					{
						int add = 0;
						for (int y = 1;y<cbCardCount;y++)
						{
							if(y != n&&y!=j)
							{
								iSingleA[add] =cbCardDataSort[y]; 
								add++;
							}
						}
						if(iBigValue<=(GetCardLogicValue(iSingleA[0])+GetCardLogicValue(iSingleA[1]))%10)
						{
							iBigValue = GetCardLogicValue(iSingleA[0])+GetCardLogicValue(iSingleA[1])%10;
							bcMakeMax[0]= cbCardDataSort[0];
							bcMakeMax[1]= cbCardDataSort[j];
							bcMakeMax[2]= cbCardDataSort[n]; 
							bcMakeMax[3]= iSingleA[0]; 
							bcMakeMax[4]= iSingleA[1]; 
							if(bcOutCadData != NULL)
							{
								CopyMemory(bcOutCadData,bcMakeMax,cbCardCount);
							}
							if(iBigValue==0)
							{
								return 10;
							}
						}
					}
				}
			}
		}
		if(iBigValue!= 0)
		{
			return (iBigValue%10==0)?(10):iBigValue%10;
		}else
		{
			//如果组合不成功
			iGetTenCount = 0;
		}

	}
	if(iGetTenCount==0)
	{
		//5个组合
		for ( n= 0;n<cbCardCount;n++)
		{
			for (int j = 0;j<cbCardCount;j++)
			{
				if(j != n)
				{
					for (int w = 0;w<cbCardCount;w++)
					{
						if(w != n&&w!=j)
						{
							int valueAdd = GetCardLogicValue(cbCardDataSort[n]);
							valueAdd += GetCardLogicValue(cbCardDataSort[j]);
							valueAdd += GetCardLogicValue(cbCardDataSort[w]);
							if(valueAdd%10==0)
							{
								int add = 0;
								for (int y = 0;y<cbCardCount;y++)
								{
									if(y != n&&y!=j&&y!=w)
									{
										iSingleA[add] =cbCardDataSort[y]; 
										add++;
									}
								}
								if(iBigValue<=(GetCardLogicValue(iSingleA[0])+GetCardLogicValue(iSingleA[1]))%10)
								{
									iBigValue = GetCardLogicValue(iSingleA[0])+GetCardLogicValue(iSingleA[1])%10;
									bcMakeMax[0]= cbCardDataSort[n];
									bcMakeMax[1]= cbCardDataSort[j];
									bcMakeMax[2]= cbCardDataSort[w]; 
									bcMakeMax[3]= iSingleA[0]; 
									bcMakeMax[4]= iSingleA[1]; 

									if(bcOutCadData != NULL)
									{
										CopyMemory(bcOutCadData,bcMakeMax,cbCardCount);
									}
									if(iBigValue==0)
									{
										return 10;
									}
								}
							}
						}
					}
				}
			}		
		}
		if(iBigValue!=0)
		{
			return (iBigValue%10==0)?(10):iBigValue%10;
		}	
		else
		{
			return 0;
		}
	}

	return 0;
}

//获取倍数
BYTE GameLogicOneByOne::GetTimes(BYTE cbCardData[], BYTE cbCardCount)
{
	if(cbCardCount!=MAX_COUNT)return 0;

	BYTE bTimes=GetCardType(cbCardData,MAX_COUNT);
	if(bTimes<10)return 1;
	else if(bTimes==10)return 2;
	/*else if(bTimes==OX_THREE_SAME)return 5;
	else if(bTimes==OX_FOUR_SAME)return 5;
	else if(bTimes==OX_FOURKING)return 5;
	else if(bTimes==OX_FIVEKING)return 5;*/

	return 0;
}

//获取牛牛
bool GameLogicOneByOne::GetOxCard(BYTE cbCardData[], BYTE cbCardCount)
{
	assert(cbCardCount==MAX_COUNT);

	//设置变量
	BYTE bTemp[MAX_COUNT],bTempData[MAX_COUNT];
	CopyMemory(bTempData,cbCardData,sizeof(bTempData));
	BYTE bSum=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		bTemp[i]=GetCardLogicValue(cbCardData[i]);
		bSum+=bTemp[i];
	}

	//查找牛牛
	for (BYTE i=0;i<cbCardCount-1;i++)
	{
		for (BYTE j=i+1;j<cbCardCount;j++)
		{
			if((bSum-bTemp[i]-bTemp[j])%10==0)
			{
				BYTE bCount=0;
				for (BYTE k=0;k<cbCardCount;k++)
				{
					if(k!=i && k!=j)
					{
						cbCardData[bCount++] = bTempData[k];
					}
				}assert(bCount==3);

				cbCardData[bCount++] = bTempData[i];
				cbCardData[bCount++] = bTempData[j];

				return true;
			}
		}
	}

	return false;
}

//获取整数
bool GameLogicOneByOne::IsIntValue(BYTE cbCardData[], BYTE cbCardCount)
{
	BYTE sum=0;
	for(BYTE i=0;i<cbCardCount;i++)
	{
		sum+=GetCardLogicValue(cbCardData[i]);
		if(GetCardLogicValue(cbCardData[i])==11)
			return true;
	}
	assert(sum>0);
	return (sum%10==0);
}

//排列扑克
void GameLogicOneByOne::SortCardList(BYTE cbCardData[], BYTE cbCardCount)
{
	//数目过虑
	if (cbCardCount==0) return;

	//转换数值
	BYTE cbSortValue[54];
	for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardNewValue(cbCardData[i]);

	//排序操作
	bool bSorted=true;
	BYTE cbThreeCount,cbLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<cbLast;i++)
		{
			if ((cbSortValue[i]<cbSortValue[i+1])||
				((cbSortValue[i]==cbSortValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
			{
				//交换位置
				cbThreeCount=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbThreeCount;
				cbThreeCount=cbSortValue[i];
				cbSortValue[i]=cbSortValue[i+1];
				cbSortValue[i+1]=cbThreeCount;
				bSorted=false;
			}	
		}
		cbLast--;
	} while(bSorted==false);

	return;
}

//混乱扑克
void GameLogicOneByOne::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//CopyMemory(cbCardBuffer,m_cbCardListData,cbBufferCount);
	//混乱准备
	BYTE cbCardData[CountArray(m_cbCardListData)];
	CopyMemory(cbCardData,m_cbCardListData,sizeof(m_cbCardListData));

	//混乱扑克
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(CountArray(m_cbCardListData)-bRandCount);
		cbCardBuffer[bRandCount++]=cbCardData[bPosition];
		cbCardData[bPosition]=cbCardData[CountArray(m_cbCardListData)-bRandCount];
	} while (bRandCount<cbBufferCount);

	return;
}

//逻辑数值
BYTE GameLogicOneByOne::GetCardLogicValue(BYTE cbCardData)
{
	//扑克属性
	BYTE bCardColor=GetCardColor(cbCardData);
	BYTE bCardValue=GetCardValue(cbCardData);

	if(bCardColor==0x4)	return 11;
	//转换数值
	return (bCardValue>10)?(10):bCardValue;
}

BYTE GameLogicOneByOne::GetCardNewValue(BYTE cbCardData)
{
	//扑克属性
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);

	//转换数值
	if (cbCardColor==0x04) return cbCardValue+13+2;
	return cbCardValue;

}
//对比扑克
bool GameLogicOneByOne::CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount,bool FirstOX,bool NextOX)
{
	if(FirstOX!=NextOX)return (FirstOX>NextOX);

	//比较牛大小
	if(FirstOX==true)
	{
		//获取点数
		BYTE cbNextType=GetCardType(cbNextData,cbCardCount);
		BYTE cbFirstType=GetCardType(cbFirstData,cbCardCount);

		//点数判断
		if (cbFirstType!=cbNextType) return (cbFirstType>cbNextType);
	}

	//排序大小
	BYTE bFirstTemp[MAX_COUNT],bNextTemp[MAX_COUNT];
	CopyMemory(bFirstTemp,cbFirstData,cbCardCount);
	CopyMemory(bNextTemp,cbNextData,cbCardCount);
	SortCardList(bFirstTemp,cbCardCount);
	SortCardList(bNextTemp,cbCardCount);

	//比较数值
	BYTE cbNextMaxValue=GetCardNewValue(bNextTemp[0]);
	BYTE cbFirstMaxValue=GetCardNewValue(bFirstTemp[0]);
	if(cbNextMaxValue!=cbFirstMaxValue)
	{
		if(GetCardColor(bFirstTemp[0])==0x04 && GetCardColor(bNextTemp[0])==0x04)
			return cbFirstMaxValue>cbNextMaxValue;
		else if(GetCardColor(bFirstTemp[0])==0x04 || GetCardColor(bNextTemp[0])==0x04)
			return cbFirstMaxValue<cbNextMaxValue;
		return cbFirstMaxValue>cbNextMaxValue;
	}

	//比较颜色
	return GetCardColor(bFirstTemp[0]) > GetCardColor(bNextTemp[0]);

	return false;
}

//////////////////////////////////////////////////////////////////////////
