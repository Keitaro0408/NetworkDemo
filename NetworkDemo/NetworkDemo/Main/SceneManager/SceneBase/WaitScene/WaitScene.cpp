#include "WaitScene.h"
#include "../../GameDataManager/GameDataManager.h"
#include "DxInput\KeyBoard\KeyDevice.h"

WaitScene::WaitScene() :
SceneBase(SCENE_WAIT),
m_IsWaitEnd(false)
{
	if (WSAStartup(MAKEWORD(2, 0), &m_WsaData) != 0)
	{
		OutputDebugString("WSAStartupに失敗しました。");
	}

	m_Socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_Socket == INVALID_SOCKET)
	{
		WSACleanup();
		OutputDebugString("ソケットの生成に失敗しました。");
	}

	FD_SET(m_Socket, &m_ReadFds);

	m_TimeOut.tv_sec = 2;
	m_TimeOut.tv_usec = 0;

	m_ServerAdd.sin_family = AF_INET;
	m_ServerAdd.sin_port = htons(PORT);
	m_ServerAdd.sin_addr.s_addr = inet_addr(IPADD);
	//m_ServerAdd.sin_addr.s_addr = inet_addr("49.250.217.198");

	m_SendData.IsMapLoad = false;
	m_SendData.IsOk = false;
	m_RecvData.IsStart = false;
	m_RecvData.Id = 0;
	m_pWaitThread = new std::thread(&WaitScene::WaitThread, this);
}


WaitScene::~WaitScene()
{
	m_IsWaitEnd = true;
	m_pWaitThread->join();
	delete m_pWaitThread;
	m_pWaitThread = NULL;

	shutdown(m_Socket, SD_BOTH);
	closesocket(m_Socket);
	WSACleanup();
}

void WaitScene::WaitThread()
{
	while (!m_IsWaitEnd)
	{
		memcpy(&m_Fds, &m_ReadFds, sizeof(fd_set));
		sendto(m_Socket, reinterpret_cast<char*>(&m_SendData), sizeof(SendData), 0, (struct sockaddr *)&m_ServerAdd, sizeof(m_ServerAdd));

		memcpy(&m_Fds, &m_ReadFds, sizeof(fd_set));
		int selectResult = select(m_Socket + 1, &m_Fds, NULL, NULL, &m_TimeOut);
		if (FD_ISSET(m_Socket, &m_Fds))
		{
			int len = (int)sizeof(sockaddr_in);
			recvfrom(m_Socket, reinterpret_cast<char*>(&m_RecvData), sizeof(RecvData), 0, (sockaddr*)&m_ServerAdd, &len);
			SINGLETON_INSTANCE(GameDataManager).SetId(m_RecvData.Id);
			if (m_RecvData.IsStart)
			{
				m_IsWaitEnd = true;
			}
		}
	}
}

WaitScene::SceneID WaitScene::Update()
{
	SINGLETON_INSTANCE(Lib::KeyDevice).Update();
	SINGLETON_INSTANCE(Lib::KeyDevice).KeyCheck(DIK_RETURN);

	if (SINGLETON_INSTANCE(Lib::KeyDevice).GetKeyState()[DIK_RETURN] == Lib::KEY_PUSH)
	{
		m_SendData.IsOk = true;
	}

	if (m_IsWaitEnd)
	{
		m_SceneID = SceneBase::SCENE_GAME;
	}

	return m_SceneID;
}

void WaitScene::Draw()
{

}
