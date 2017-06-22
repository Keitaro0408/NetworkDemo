#include "SceneManager/SceneManager.h"
#include "Application/ApplicationBase.h"
#include "Window/Window.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

class App : Lib::ApplicationBase
{
public:
	App() :
		ApplicationBase("NetworkDemo", 1280, 720){};
	~App() = default;

	void Init()
	{
		m_pSceneManager = new SceneManager();
	};

	void Release()
	{
		delete m_pSceneManager;
		m_pSceneManager = NULL;
	};

	bool MainLoop()
	{
		if (m_pSceneManager->Run())
		{
			return true;
		}
		return false;
	};

private:
	SceneManager* m_pSceneManager;

};

App app;
