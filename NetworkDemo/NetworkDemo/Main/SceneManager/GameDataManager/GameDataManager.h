/**
 * @file   GameDataManager.h
 * @brief  GameDataManagerクラスのヘッダファイル
 * @author kotani
 */
#ifndef GAMEDATAMANAGER_H
#define GAMEDATAMANAGER_H
#include <string>
#include <thread>


class GameDataManager
{
public:

	GameDataManager();
	~GameDataManager();

	inline void SetId(int32_t _id)
	{
		m_Id = _id;
	}

	inline int32_t  GetId()
	{
		return m_Id;
	}

private:
	int32_t m_Id;

};


#endif
