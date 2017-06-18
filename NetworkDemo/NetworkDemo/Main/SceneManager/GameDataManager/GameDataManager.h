﻿/**
 * @file   GameDataManager.h
 * @brief  GameDataManagerクラスのヘッダファイル
 * @author kotani
 */
#ifndef GAMEDATAMANAGER_H
#define GAMEDATAMANAGER_H

#define IPADD "192.168.12.47"
#define PORT 50000


#include <string>
#include <thread>
#include "Singleton.h"


class GameDataManager
{
	friend Lib::Singleton<GameDataManager>;
public:
	inline void SetId(int32_t _id)
	{
		m_Id = _id;
	}

	inline int32_t  GetId()
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


private:
	GameDataManager();
	~GameDataManager();
	int32_t m_Id;
	int		m_PlayerNum;

};


#endif
