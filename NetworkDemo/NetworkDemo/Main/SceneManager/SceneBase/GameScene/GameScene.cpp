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

namespace
{
	D3DXVECTOR2 g_UV[4];
	D3DXVECTOR2 g_Pos[2];
}

GameScene::GameScene() :
SceneBase(SCENE_GAME)
{
	//m_pUdpThread = new UdpThread("49.250.217.198", 12345);
	//m_pUdpThread = new UdpThread("192.168.12.48", 12345);
	m_pUdpThread = new UdpThread("192.168.12.46", 50000);
	g_Pos[0] = D3DXVECTOR2(100, 100);
	g_Pos[1] = D3DXVECTOR2(100, 100);

	SINGLETON_INSTANCE(Lib::TextureManager).Load("Resource/test.jpg", &m_TextureIndex);
	m_pVertex[0] = new Lib::Vertex2D(
		SINGLETON_INSTANCE(Lib::DX11Manager).GetDevice(),
		SINGLETON_INSTANCE(Lib::DX11Manager).GetDeviceContext(),
		SINGLETON_INSTANCE(Lib::Window).GetWindowSize());

	m_pVertex[1] = new Lib::Vertex2D(
		SINGLETON_INSTANCE(Lib::DX11Manager).GetDevice(),
		SINGLETON_INSTANCE(Lib::DX11Manager).GetDeviceContext(),
		SINGLETON_INSTANCE(Lib::Window).GetWindowSize());
	g_UV[0].x = 0.f;
	g_UV[0].y = 0.f;

	g_UV[1].x = 1.f;
	g_UV[1].y = 0.f;

	g_UV[2].x = 0.f;
	g_UV[2].y = 1.f;

	g_UV[3].x = 1.f;
	g_UV[3].y = 1.f;
	m_pVertex[0]->Init(&D3DXVECTOR2(100, 100), g_UV);
	m_pVertex[0]->SetTexture(
		SINGLETON_INSTANCE(Lib::TextureManager).GetTexture(m_TextureIndex));
	
	m_pVertex[1]->Init(&D3DXVECTOR2(100, 100), g_UV);
	m_pVertex[1]->SetTexture(
		SINGLETON_INSTANCE(Lib::TextureManager).GetTexture(m_TextureIndex));

}

GameScene::~GameScene()
{
	m_pVertex[1]->Release();
	delete m_pVertex[1];
	m_pVertex[1] = NULL;

	m_pVertex[0]->Release();
	delete m_pVertex[0];
	m_pVertex[0] = NULL;
	SINGLETON_INSTANCE(Lib::TextureManager).ReleaseTexture(m_TextureIndex);
	delete m_pUdpThread;
	m_pUdpThread = NULL;}


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
		g_Pos[0].x = m_pUdpThread->GetRecvData().PlayerData[0].PosX;
		g_Pos[0].y = m_pUdpThread->GetRecvData().PlayerData[0].PosY;
		g_Pos[1].x = m_pUdpThread->GetRecvData().PlayerData[1].PosX;
		g_Pos[1].y = m_pUdpThread->GetRecvData().PlayerData[1].PosY;
		m_pUdpThread->SetIsUpdate(false);
	}

	SINGLETON_INSTANCE(Lib::XInput).Update(Lib::GAMEPAD1);
	return m_SceneID;
}

void GameScene::Draw()
{
	SINGLETON_INSTANCE(Lib::DX11Manager).SetDepthStencilTest(false);
	SINGLETON_INSTANCE(Lib::DX11Manager).BeginScene();
	m_pVertex[0]->Draw(&D3DXVECTOR2(g_Pos[0].x + 100, g_Pos[0].y + 100), g_UV);
	m_pVertex[1]->Draw(&D3DXVECTOR2(g_Pos[1].x + 200, g_Pos[1].y + 100), g_UV);
	SINGLETON_INSTANCE(Lib::DX11Manager).EndScene();
}
