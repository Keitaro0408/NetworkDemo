/**
 * @file   NetworkDataManager.h
 * @brief  NetworkDataManagerクラスのヘッダファイル
 * @author kotani
 */
#ifndef GAMEDATAMANAGER_H
#define GAMEDATAMANAGER_H

//#define IPADD "192.168.12.46"
//#define IPADD "192.168.12.48"
//#define IPADD "49.250.217.198"
#define PORT 50000


#include <string>
#include <thread>
#include "Singleton.h"


class NetworkDataManager
{
	friend Lib::Singleton<NetworkDataManager>;
public:
	inline void SetId(unsigned char _id)
	{
		m_Id = _id;
	}

	inline unsigned char GetId()
	{
		return m_Id;
	}

	inline void SetPlayerNum(int32_t _id)
	{
		m_PlayerNum = _id;
	}

	inline int GetPlayerNum()
	{
		return m_PlayerNum;
	}

	inline void SetPort(u_short _port)
	{
		m_Port = _port;
	}

	inline int GetPort()
	{
		return m_Port;
	}

	inline const char* GetIp()
	{
		return m_Ip;
	}

private:
	NetworkDataManager();
	~NetworkDataManager();
	unsigned char m_Id;
	u_short	m_Port;
	int		m_PlayerNum;

	char    m_Ip[16];
};


#endif
