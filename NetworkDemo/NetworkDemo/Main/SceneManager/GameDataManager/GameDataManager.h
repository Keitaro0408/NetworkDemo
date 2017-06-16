/**
 * @file   GameDataManager.h
 * @brief  GameDataManagerクラスのヘッダファイル
 * @author kotani
 */
#ifndef GAMEDATAMANAGER_H
#define GAMEDATAMANAGER_H

#define IPADD "127.0.0.1"
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

private:
	GameDataManager();
	~GameDataManager();
	int32_t m_Id;

};


#endif
