/**
 * @file   GameScene.h
 * @brief  GameSceneクラスのヘッダファイル
 * @author kotani
 */
#ifndef GAMESCENE_H
#define GAMESCENE_H
#include <memory>
#include "..\SceneBase.h"
#include "Vertex2D\Vertex2D.h"
#include <vector>

class UdpThread;
class ObjectManager;

/**
 * ゲームシーンを管理するクラス
 */
class GameScene : public SceneBase
{
public:
	/**
	 * コンストラクタ
	 */
	GameScene();

	/**
	 * デストラクタ
	 */
	virtual ~GameScene();

	/**
	 * 制御関数
	 * @return シーンの遷移先ID
	 */
	SceneID Update() override;

	/**
	 * 描画関数
	 */
	void Draw() override;

private:
	GameScene(const GameScene&);
	void operator=(const GameScene&);

	ObjectManager* m_pObjectManager;
	UdpThread*	   m_pUdpThread;
	std::vector<Lib::Vertex2D*> m_pVertex;
	int			   m_TextureIndex;

};


#endif
