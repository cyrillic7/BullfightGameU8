#ifndef PACKET_AIDE_HEAD_FILE
#define PACKET_AIDE_HEAD_FILE
#include "Define.h"
#include "cocos2d.h"
#pragma once

/*//如果是android平台需要定义宏
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID||CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
#define RtlCopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
#define CopyMemory RtlCopyMemory
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef char TCHAR, *PTCHAR;
#endif*/

//存储长度
#define CountStringBufferA(String) ((UINT)((lstrlenA(String)+1)*sizeof(CHAR)))
#define CountStringBufferW(String) ((UINT)((lstrlenW(String)+1)*sizeof(WCHAR)))
//数组维数
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////

//数据定义
#define DTP_NULL					0								//无效数据

//数据描述
struct tagDataDescribe
{
	WORD							wDataSize;						//数据大小
	WORD							wDataDescribe;					//数据描述
};
/*
//////////////////////////////////////////////////////////////////////////////////

//发送辅助类
class CSendPacketHelper
{
	//变量定义
protected:
	WORD							m_wDataSize;					//数据大小
	WORD							m_wMaxBytes;					//缓冲大小
	LPBYTE							m_pcbBuffer;					//缓冲指针

	//函数定义
public:
	//构造函数
	inline CSendPacketHelper(void * pcbBuffer, WORD wMaxBytes);

	//功能函数
public:
	//清理数据
	inline void CleanData() { m_wDataSize=0; }
	//获取大小
	inline WORD GetDataSize() { return m_wDataSize; }
	//获取缓冲
	inline void * GetDataBuffer() { return m_pcbBuffer; }

	//功能函数
public:
	//插入字符
	inline bool AddPacket(LPCSTR pszString, WORD wDataType);
	//插入字符
	inline bool AddPacket(LPCWSTR pszString, WORD wDataType);
	//插入数据
	inline bool AddPacket(void * pData, WORD wDataSize, WORD wDataType);
};

//////////////////////////////////////////////////////////////////////////////////

//接收辅助类
class CRecvPacketHelper
{
	//变量定义
protected:
	WORD							m_wDataPos;						//数据点
	WORD							m_wDataSize;					//数据大小
	LPBYTE							m_pcbBuffer;					//缓冲指针

	//函数定义
public:
	//构造函数
	inline CRecvPacketHelper(LPCVOID pcbBuffer, WORD wDataSize);

	//功能函数
public:
	//获取数据
	inline void * GetData(tagDataDescribe & DataDescribe);
};

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSendPacketHelper::CSendPacketHelper(void * pcbBuffer, WORD wMaxBytes)
{
	//设置变量
	m_wDataSize=0;
	m_wMaxBytes=wMaxBytes;
	m_pcbBuffer=(BYTE *)pcbBuffer;

	return;
}

//插入字符
bool CSendPacketHelper::AddPacket(LPCSTR pszString, WORD wDataType)
{
	//参数判断
	assert(pszString!=NULL);
	if (pszString[0]==0) return true;

	//插入数据
	return AddPacket((void *)pszString,CountStringBufferA(pszString),wDataType);
}

//插入字符
bool CSendPacketHelper::AddPacket(LPCWSTR pszString, WORD wDataType)
{
	//参数判断
	assert(pszString!=NULL);
	if (pszString[0]==0) return true;

	//插入数据
	return AddPacket((void *)pszString,CountStringBufferW(pszString),wDataType);
}

//插入数据
bool CSendPacketHelper::AddPacket(void * pData, WORD wDataSize, WORD wDataType)
{
	//效验数据
	assert(wDataType!=DTP_NULL);
	assert((wDataSize+sizeof(tagDataDescribe)+m_wDataSize)<=m_wMaxBytes);
	if ((wDataSize+sizeof(tagDataDescribe)+m_wDataSize)>m_wMaxBytes) return false;

	//插入数据
	assert(m_pcbBuffer!=NULL);
	tagDataDescribe * pDataDescribe=(tagDataDescribe *)(m_pcbBuffer+m_wDataSize);
	pDataDescribe->wDataSize=wDataSize;
	pDataDescribe->wDataDescribe=wDataType;

	//插入数据
	if (wDataSize>0)
	{
		assert(pData!=NULL);
		CopyMemory(pDataDescribe+1,pData,wDataSize);
	}

	//设置数据
	m_wDataSize+=sizeof(tagDataDescribe)+wDataSize;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CRecvPacketHelper::CRecvPacketHelper(LPCVOID pcbBuffer, WORD wDataSize)
{
	//设置变量
	m_wDataPos=0;
	m_wDataSize=wDataSize;
	m_pcbBuffer=(BYTE *)pcbBuffer;

	return;
}

//获取数据
VOID * CRecvPacketHelper::GetData(tagDataDescribe & DataDescribe)
{
	//效验数据
	if (m_wDataPos>=m_wDataSize) 
	{
		assert(m_wDataPos==m_wDataSize);
		DataDescribe.wDataSize=0;
		DataDescribe.wDataDescribe=DTP_NULL;
		return NULL;
	}

	//获取数据
	assert((m_wDataPos+sizeof(tagDataDescribe))<=m_wDataSize);
	CopyMemory(&DataDescribe,m_pcbBuffer+m_wDataPos,sizeof(tagDataDescribe));
	assert((m_wDataPos+sizeof(tagDataDescribe)+DataDescribe.wDataSize)<=m_wDataSize);

	//效验数据
	if ((m_wDataPos+sizeof(tagDataDescribe)+DataDescribe.wDataSize)>m_wDataSize)
	{
		DataDescribe.wDataSize=0;
		DataDescribe.wDataDescribe=DTP_NULL;
		return NULL;
	}

	//设置数据
	VOID * pData=NULL;
	if (DataDescribe.wDataSize>0) pData=m_pcbBuffer+m_wDataPos+sizeof(tagDataDescribe);
	m_wDataPos+=sizeof(tagDataDescribe)+DataDescribe.wDataSize;

	return pData;
};*/


//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif
