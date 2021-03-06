/**
 * @file   ObjectManager.cpp
 * @brief  ObjectManagerクラスの実装
 * @author kotani
 */
#include "../UdpThread/UdpThread.h"
#include "ObjectManager.h"
#include "ObjectBase/Map/Map.h"
#include "Helper/Helper.h"
#include "../../../NetworkDataManager/NetworkDataManager.h"
#include "Texture/TextureManager.h"
#include "ObjectBase/Player/Player.h"


ObjectManager::ObjectManager()
{
	SINGLETON_INSTANCE(Lib::TextureManager).Load("Resource/chara.png",&m_PlayerTextureIndex);
	m_pObjectBase.push_back(new Map());
	int playerNum = SINGLETON_INSTANCE(NetworkDataManager).GetPlayerNum();
	for (int i = 0; i < playerNum; i++)
	{
		m_pPlayer.push_back(new Player(m_PlayerTextureIndex));
	}
}


ObjectManager::~ObjectManager()
{
	for (int i = m_pPlayer.size() - 1; i >= 0; i--)
	{
		Lib::SafeDelete(m_pPlayer[i]);
	}

	for (int i = m_pObjectBase.size() - 1; i >= 0; i--)
	{
		Lib::SafeDelete(m_pObjectBase[i]);
	}
	SINGLETON_INSTANCE(Lib::TextureManager).ReleaseTexture(m_PlayerTextureIndex);
}


//----------------------------------------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------------------------------------

void ObjectManager::Update()
{
	for (unsigned int i = 0; i < m_pObjectBase.size(); i++)
	{
		m_pObjectBase[i]->Update();
	}

	D3DXVECTOR2 pos;
	if (SINGLETON_INSTANCE(UdpThread).GetIsUpdate())
	{
		for (unsigned int i = 0; i < m_pPlayer.size(); i++)
		{
			m_pPlayer[i]->SetPlayerData(SINGLETON_INSTANCE(UdpThread).GetRecvData()[i]);
			m_pPlayer[i]->Update();
		}
		SINGLETON_INSTANCE(UdpThread).SetIsUpdate(false);
	}
}

void ObjectManager::Draw()
{
	for (unsigned int i = 0; i < m_pObjectBase.size(); i++)
	{
		m_pObjectBase[i]->Draw();
	}

	int playerId = SINGLETON_INSTANCE(NetworkDataManager).GetId();

	for (unsigned int i = 0; i < m_pPlayer.size(); i++)
	{
		if (i != playerId - 1)
		{
			m_pPlayer[i]->Draw();
		}
	}
	m_pPlayer[playerId - 1]->IconDraw();
	m_pPlayer[playerId - 1]->Draw();

}
