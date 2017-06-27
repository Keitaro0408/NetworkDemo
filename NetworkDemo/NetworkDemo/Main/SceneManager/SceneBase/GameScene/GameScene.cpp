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
#include "ObjectManager/ObjectManager.h"

namespace
{
	D3DXVECTOR2 g_UV[4];
	std::vector<D3DXVECTOR2> g_Pos;
}

GameScene::GameScene() :
SceneBase(SCENE_GAME)
{
	//m_pUdpThread = new UdpThread("49.250.217.198", 50000);
	SINGLETON_CREATE(UdpThread);

	SINGLETON_INSTANCE(UdpThread).Init(SINGLETON_INSTANCE(GameDataManager).GetIp(), PORT);
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

	m_pObjectManager = new ObjectManager();

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

	delete m_pObjectManager;
	
	SINGLETON_INSTANCE(Lib::TextureManager).ReleaseTexture(m_TextureIndex);

	SINGLETON_DELETE(UdpThread);
}


//----------------------------------------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------------------------------------

SceneBase::SceneID GameScene::Update()
{
	SINGLETON_INSTANCE(Lib::KeyDevice).Update();
	SINGLETON_INSTANCE(Lib::KeyDevice).KeyCheck(DIK_UPARROW);
	SINGLETON_INSTANCE(Lib::KeyDevice).KeyCheck(DIK_LEFTARROW);
	SINGLETON_INSTANCE(Lib::KeyDevice).KeyCheck(DIK_RIGHTARROW);
	SINGLETON_INSTANCE(Lib::KeyDevice).KeyCheck(DIK_Z);
	m_pObjectManager->Update();

	SINGLETON_INSTANCE(Lib::XInput).Update(Lib::GAMEPAD1);
	return m_SceneID;
}

void GameScene::Draw()
{
	SINGLETON_INSTANCE(Lib::DX11Manager).SetDepthStencilTest(false);
	SINGLETON_INSTANCE(Lib::DX11Manager).BeginScene();
	m_pObjectManager->Draw();
	SINGLETON_INSTANCE(Lib::DX11Manager).EndScene();
}
