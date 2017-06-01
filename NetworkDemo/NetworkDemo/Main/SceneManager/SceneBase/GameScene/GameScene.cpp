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
	D3DXVECTOR2 g_Pos;
}

GameScene::GameScene() :
SceneBase(SCENE_GAME)
{
	{
		const HWND hWnd = SINGLETON_INSTANCE(Lib::Window).GetWindowHandle();

		// Lib::DSoundManager Init
		SINGLETON_CREATE(Lib::DSoundManager);
		SINGLETON_INSTANCE(Lib::DSoundManager).Init(hWnd);
		// Lib::DSoundManager Init end

		SINGLETON_CREATE(Lib::DXInputDevice);

		// InputDevice関係
		SINGLETON_CREATE(Lib::DXInputDevice);
		SINGLETON_INSTANCE(Lib::DXInputDevice).Init(hWnd);

		SINGLETON_CREATE(Lib::MouseDevice);
		SINGLETON_INSTANCE(Lib::MouseDevice).Init(
			SINGLETON_INSTANCE(Lib::DXInputDevice).GetInputDevice(), hWnd);

		SINGLETON_CREATE(Lib::KeyDevice);
		SINGLETON_INSTANCE(Lib::KeyDevice).Init(
			SINGLETON_INSTANCE(Lib::DXInputDevice).GetInputDevice(), hWnd);

		SINGLETON_CREATE(Lib::XInput);
	}

	{
		ID3D11Device* const pDevice = SINGLETON_INSTANCE(Lib::DX11Manager).GetDevice();

		// Lib::TextureManager Init
		SINGLETON_CREATE(Lib::TextureManager);
		SINGLETON_INSTANCE(Lib::TextureManager).Init(pDevice);
		// Lib::TextureManager Init end
	}

	//m_pUdpThread = new UdpThread("49.250.217.198", 12345);
	m_pUdpThread = new UdpThread("192.168.12.33", 12345);
	g_Pos = D3DXVECTOR2(100, 100);
	SINGLETON_INSTANCE(Lib::TextureManager).Load("Resource/test.jpg", &m_TextureIndex);
	m_pVertex = new Lib::Vertex2D(
		SINGLETON_INSTANCE(Lib::DX11Manager).GetDevice(),
		SINGLETON_INSTANCE(Lib::DX11Manager).GetDeviceContext(),
		SINGLETON_INSTANCE(Lib::Window).GetWindowHandle());

	g_UV[0].x = 0.f;
	g_UV[0].y = 0.f;

	g_UV[1].x = 1.f;
	g_UV[1].y = 0.f;

	g_UV[2].x = 0.f;
	g_UV[2].y = 1.f;

	g_UV[3].x = 1.f;
	g_UV[3].y = 1.f;
	m_pVertex->Init(&D3DXVECTOR2(100, 100), g_UV);
	m_pVertex->SetTexture(
		SINGLETON_INSTANCE(Lib::TextureManager).GetTexture(m_TextureIndex));

}

GameScene::~GameScene()
{
	m_pVertex->Release();
	delete m_pVertex;
	m_pVertex = NULL;
	SINGLETON_INSTANCE(Lib::TextureManager).ReleaseTexture(m_TextureIndex);
	delete m_pUdpThread;
	m_pUdpThread = NULL;

	// Lib::TextureManager Delete
	SINGLETON_INSTANCE(Lib::TextureManager).Release();
	SINGLETON_DELETE(Lib::TextureManager);
	// Lib::TextureManager Delete end

	SINGLETON_DELETE(Lib::XInput);

	// Lib::InputDevice関係
	SINGLETON_INSTANCE(Lib::KeyDevice).Release();
	SINGLETON_DELETE(Lib::KeyDevice);

	SINGLETON_INSTANCE(Lib::MouseDevice).Release();
	SINGLETON_DELETE(Lib::MouseDevice);

	SINGLETON_INSTANCE(Lib::DXInputDevice).Release();
	SINGLETON_DELETE(Lib::DXInputDevice);

	// Lib::DSoundManager Delete
	SINGLETON_INSTANCE(Lib::DSoundManager).Release();
	SINGLETON_DELETE(Lib::DSoundManager);
	// Lib::DSoundManager Delete end

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

	SINGLETON_INSTANCE(Lib::XInput).Update(Lib::GAMEPAD1);
	return m_SceneID;
}

void GameScene::Draw()
{
	SINGLETON_INSTANCE(Lib::DX11Manager).SetDepthStencilTest(false);
	SINGLETON_INSTANCE(Lib::DX11Manager).BeginScene();
	g_Pos.x = m_pUdpThread->GetRecvData().PlayerData.PosX;
	g_Pos.y = m_pUdpThread->GetRecvData().PlayerData.PosY;

	m_pVertex->Draw(&D3DXVECTOR2(g_Pos.x + 100, g_Pos.y + 100), g_UV);
	SINGLETON_INSTANCE(Lib::DX11Manager).EndScene();
}
