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
	m_TimeOut.tv_sec = 5;
	m_TimeOut.tv_usec = 0;

	m_ServerAdd.sin_family = AF_INET;
	m_ServerAdd.sin_port = htons(PORT);
	m_ServerAdd.sin_addr.s_addr = INADDR_ANY;
	m_Socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_Socket == INVALID_SOCKET)
	{
		WSACleanup();
		OutputDebugString("ソケットの生成に失敗しました。");
	}

	for (int i = 0; i < 20;i++)
	{
		m_ServerAdd.sin_port = htons(PORT + i);
		int r = bind(m_Socket, (struct sockaddr *)&m_ServerAdd, sizeof(m_ServerAdd));
		if (r == -1)
		{
				OutputDebugString("bindに失敗\n。");
		}
		else
		{
			SINGLETON_INSTANCE(GameDataManager).SetPort(PORT + i);
			break;
		}
	}


	FD_ZERO(&m_ReadFds);
	FD_SET(m_Socket, &m_ReadFds);

	m_ServerAdd.sin_addr.s_addr = inet_addr(SINGLETON_INSTANCE(GameDataManager).GetIp());
	m_ServerAdd.sin_port = htons(PORT);
	m_SendData.IsMapLoad = false;
	m_SendData.IsOk = false;
	m_SendData.Id = 0;
	m_RecvData.IsStart = false;
	m_RecvData.Id = 0;
	SINGLETON_INSTANCE(Lib::KeyDevice).Update();
	SINGLETON_INSTANCE(Lib::KeyDevice).KeyCheck(DIK_RETURN);

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
	int len = (int)sizeof(sockaddr_in);
	sendto(m_Socket, reinterpret_cast<char*>(&m_SendData), sizeof(SendData), 0, (struct sockaddr *)&m_ServerAdd, sizeof(m_ServerAdd));

	memcpy(&m_Fds, &m_ReadFds, sizeof(fd_set));
	select(m_Socket + 1, &m_Fds, NULL, NULL, &m_TimeOut);

	while (!m_IsWaitEnd)
	{
		memcpy(&m_Fds, &m_ReadFds, sizeof(fd_set));
		select(m_Socket + 1, &m_Fds, NULL, NULL, &m_TimeOut);
		if (FD_ISSET(m_Socket, &m_Fds))
		{
			recvfrom(m_Socket, reinterpret_cast<char*>(&m_RecvData), sizeof(RecvData), 0, (sockaddr*)&m_ServerAdd, &len);
			SINGLETON_INSTANCE(GameDataManager).SetId(m_RecvData.Id);
			SINGLETON_INSTANCE(GameDataManager).SetPlayerNum(m_RecvData.PlayerNum);
			OutputDebugString("recv\n");
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
		m_SendData.Id = SINGLETON_INSTANCE(GameDataManager).GetId();
		m_SendData.IsOk = true;
		sendto(m_Socket, reinterpret_cast<char*>(&m_SendData), sizeof(SendData), 0, (struct sockaddr *)&m_ServerAdd, sizeof(m_ServerAdd));
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
