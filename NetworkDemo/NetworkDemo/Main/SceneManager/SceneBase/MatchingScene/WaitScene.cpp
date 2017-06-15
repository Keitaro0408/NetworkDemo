#include "WaitScene.h"


WaitScene::WaitScene() :
SceneBase(SCENE_GAME),
m_GameIsEnd(false)
{
	if (WSAStartup(MAKEWORD(2, 0), &m_WsaData) != 0)
	{
		OutputDebugString("WSAStartup�Ɏ��s���܂����B");
	}

	m_Socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_Socket == INVALID_SOCKET)
	{
		WSACleanup();
		OutputDebugString("�\�P�b�g�̐����Ɏ��s���܂����B");
	}

	FD_SET(m_Socket, &m_ReadFds);

	m_TimeOut.tv_sec = 2;
	m_TimeOut.tv_usec = 0;

	m_ServerAdd.sin_family = AF_INET;
	m_ServerAdd.sin_port = htons(50000);
	m_ServerAdd.sin_addr.s_addr = inet_addr("49.250.217.198");

	m_pThread = new std::thread(&WaitScene::MainLoop, this);
}


WaitScene::~WaitScene()
{
	m_GameIsEnd = true;
	m_pThread->join();
	delete m_pThread;
	m_pThread = NULL;

	shutdown(m_Socket, SD_BOTH);
	closesocket(m_Socket);
	WSACleanup();
}

void WaitScene::MainLoop()
{
	while (!m_GameIsEnd)
	{
		memcpy(&m_Fds, &m_ReadFds, sizeof(fd_set));

		int len = (int)sizeof(sockaddr_in);
		int selectResult = select(m_Socket + 1, &m_Fds, NULL, NULL, &m_TimeOut);
		if (FD_ISSET(m_Socket, &m_Fds))
		{
			/* sock1����f�[�^����M���ĕ\�����܂� */
			int32_t test = 5;
			recvfrom(m_Socket, reinterpret_cast<char*>(&test), sizeof(test), 0, (sockaddr*)&m_ServerAdd, &len);

		}
	}
}

WaitScene::SceneID WaitScene::Update()
{
	return m_SceneID;
}

void WaitScene::Draw()
{

}
