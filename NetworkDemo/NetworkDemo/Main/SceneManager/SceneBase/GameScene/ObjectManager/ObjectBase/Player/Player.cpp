/**
 * @file   Player.cpp
 * @brief  Playerクラスの実装
 * @author kotani
 */
#include "Player.h"
#include "Texture/TextureManager.h"
#include "DxInput/KeyBoard/KeyDevice.h"


Player::Player(int _textureIndex) :
m_TextureIndex(_textureIndex)
{
	InitVertex(
		D3DXVECTOR2(0, 0),
		D3DXVECTOR2(64, 64),
		"Resource/chara.anim",
		"Wait",
		&m_Vertex);

	InitVertex(
		D3DXVECTOR2(0, 0),
		D3DXVECTOR2(32, 32),
		"Resource/PlayerIcon.anim",
		"Normal",
		&m_PlayerIcon);

	SINGLETON_INSTANCE(Lib::TextureManager).Load("Resource/PlayerIcon.png",&m_IconTextureIndex);

	m_PlayerIcon.pVertex->SetTexture(
		SINGLETON_INSTANCE(Lib::TextureManager).GetTexture(m_IconTextureIndex));

	m_Vertex.pVertex->SetTexture(
		SINGLETON_INSTANCE(Lib::TextureManager).GetTexture(m_TextureIndex));
	m_Vertex.pUvController->SetAnimFrame(20);
}


Player::~Player()
{
	SINGLETON_INSTANCE(Lib::TextureManager).ReleaseTexture(m_IconTextureIndex);
	ReleaseVertex(&m_PlayerIcon);
	ReleaseVertex(&m_Vertex);
}

void Player::Update()
{
	m_Vertex.pUvController->Control(false,Lib::ANIM_LOOP);
}

void Player::Draw()
{
	D3DXVECTOR2 pos;
	pos.x = m_PlayerData.PosX;
	pos.y = m_PlayerData.PosY;

	D3DXVECTOR2 Uv[4];
	memcpy(Uv, m_Vertex.pUvController->GetUV(), sizeof(D3DXVECTOR2) * 4);

	if (!m_PlayerData.IsRight)
	{
		InvertUv(Uv);
	}

	m_Vertex.pVertex->Draw(&pos, Uv);
}

void Player::IconDraw()
{
	D3DXVECTOR2 pos;
	pos.x = m_PlayerData.PosX;
	pos.y = m_PlayerData.PosY - 40.f;

	m_PlayerIcon.pVertex->Draw(&pos, m_PlayerIcon.pUvController->GetUV());
}

void Player::InvertUv(D3DXVECTOR2* _uv)
{
	D3DXVECTOR2 tmpUV[4];

	for (int i = 0; i < 4; i++)
	{
		tmpUV[i] = _uv[i];
	}

	_uv[0] = tmpUV[1];
	_uv[1] = tmpUV[0];
	_uv[2] = tmpUV[3];
	_uv[3] = tmpUV[2];
}
