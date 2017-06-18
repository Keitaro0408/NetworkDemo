/**
 * @file   GameScene.cpp
 * @brief  GameSceneクラスの実装
 * @author kotani
 */
#include "UdpThread\UdpThread.h"
#include "GameScene.h"
#include "Window/Window.h"
#include "Texture/TextureManager.h"
#include "Sound/DSoundManager.h"
#include "DxInput/KeyBoard/KeyDevice.h"
#include "Dx11/DX11Manager.h"
#include "DxInput/DXInputDevice.h"
#include "../../../XInput/XInput.h"
#include "../../GameDataManager/GameDataManager.h"

namespace
{
	D3DXVECTOR2 g_UV[4];
	std::vector<D3DXVECTOR2> g_Pos;
}

GameScene::GameScene() :
SceneBase(SCENE_GAME)
{
	//m_pUdpThread = new UdpThread("49.250.217.198", 50000);
	m_pUdpThread = new UdpThread(IPADD, PORT);
	//m_pUdpThread = new UdpThread("192.168.12.47", 50000);
	SINGLETON_INSTANCE(Lib::TextureManager).Load("Resource/test.jpg", &m_TextureIndex);
	
	g_UV[0].x = 0.f;
	g_UV[0].y = 0.f;

	g_UV[1].x = 1.f;
	g_UV[1].y = 0.f;

	g_UV[2].x = 0.f;
	g_UV[2].y = 1.f;

	g_UV[3].x = 1.f;
	g_UV[3].y = 1.f;

	int playerNum = SINGLETON_INSTANCE(GameDataManager).GetPlayerNum();
	for (int i = 0; i < playerNum; i++)
	{
		g_Pos.push_back(D3DXVECTOR2(100, 100));
		m_pVertex.push_back(new Lib::Vertex2D(
			SINGLETON_INSTANCE(Lib::DX11Manager).GetDevice(),
			SINGLETON_INSTANCE(Lib::DX11Manager).GetDeviceContext(),
			SINGLETON_INSTANCE(Lib::Window).GetWindowSize()));
		m_pVertex[i]->Init(&D3DXVECTOR2(100, 100), g_UV);
		m_pVertex[i]->SetTexture(
			SINGLETON_INSTANCE(Lib::TextureManager).GetTexture(m_TextureIndex));
	}
}

GameScene::~GameScene()
{
	for (int i = 0; i < SINGLETON_INSTANCE(GameDataManager).GetPlayerNum(); i++)
	{
		m_pVertex[i]->Release();
		delete m_pVertex[i];
		m_pVertex[i] = NULL;
	}

	SINGLETON_INSTANCE(Lib::TextureManager).ReleaseTexture(m_TextureIndex);
	delete m_pUdpThread;
	m_pUdpThread = NULL;
}


//----------------------------------------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------------------------------------

SceneBase::SceneID GameScene::Update()
{
	SINGLETON_INSTANCE(Lib::KeyDevice).Update();
	SINGLETON_INSTANCE(Lib::KeyDevice).KeyCheck(DIK_W);
	SINGLETON_INSTANCE(Lib::KeyDevice).KeyCheck(DIK_A);
	SINGLETON_INSTANCE(Lib::KeyDevice).KeyCheck(DIK_S);
	SINGLETON_INSTANCE(Lib::KeyDevice).KeyCheck(DIK_D);
	
	if (m_pUdpThread->GetIsUpdate())
	{
		for (int i = 0; i < SINGLETON_INSTANCE(GameDataManager).GetPlayerNum(); i++)
		{
			g_Pos[i].x = m_pUdpThread->GetRecvData()[i].PosX;
			g_Pos[i].y = m_pUdpThread->GetRecvData()[i].PosY;
		}
		m_pUdpThread->SetIsUpdate(false);
	}

	SINGLETON_INSTANCE(Lib::XInput).Update(Lib::GAMEPAD1);
	return m_SceneID;
}

void GameScene::Draw()
{
	SINGLETON_INSTANCE(Lib::DX11Manager).SetDepthStencilTest(false);
	SINGLETON_INSTANCE(Lib::DX11Manager).BeginScene();
	for (int i = 0; i < SINGLETON_INSTANCE(GameDataManager).GetPlayerNum(); i++)
	{
		if (i == 2)
		{
			int idsad = 0;
		}
		m_pVertex[i]->Draw(&D3DXVECTOR2(g_Pos[i].x + 100, g_Pos[i].y + 100), g_UV);
	}
	SINGLETON_INSTANCE(Lib::DX11Manager).EndScene();
}
