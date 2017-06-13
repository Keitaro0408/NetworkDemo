#include "SceneManager/SceneManager.h"
#include "Application/ApplicationBase.h"
#include "Window/Window.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

class App : Lib::ApplicationBase
{
public:
	App() = default;
	~App() = default;

	void Init()
	{
		SINGLETON_INSTANCE(Lib::Window).DispWindow(WINDOW_WIDTH, WINDOW_HEIGHT, TEXT("NetworkDemo"));
		InitLib(SINGLETON_INSTANCE(Lib::Window).GetWindowHandle());
		m_pSceneManager = new SceneManager(SINGLETON_INSTANCE(Lib::Window).GetWindowHandle());
	};

	void Release()
	{
		delete m_pSceneManager;
		m_pSceneManager = NULL;
		ReleaseLib();
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
