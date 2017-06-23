/**
 * @file   Player.h
 * @brief  Playerクラスのヘッダファイル
 * @author kotani
 */
#ifndef PLAYER_H
#define PLAYER_H
#include "../../../UdpThread/UdpThread.h"
#include "../ObjectBase.h"

class Player : public ObjectBase
{
public:
	Player(int _textureIndex);
	~Player();

	void Update() override;

	void Draw() override;

	void IconDraw();

	inline void SetPlayerData(const UdpThread::PlayerData& _playerData)
	{
		m_PlayerData = _playerData;
	}

private:
	void BulletDraw();
	void InvertUv(D3DXVECTOR2* _uv);

	ObjectBase::Vertex    m_Vertex;
	ObjectBase::Vertex    m_BulletVertex;
	ObjectBase::Vertex    m_PlayerIcon;
	int				      m_TextureIndex;
	int					  m_IconTextureIndex;
	int					  m_BulletTextureIndex;
	UdpThread::PlayerData m_PlayerData;

};


#endif
