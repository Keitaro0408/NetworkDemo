#ifndef OBJECTBASE_H
#define OBJECTBASE_H
#include "Vertex2D/Vertex2D.h"
#include "Animation/AnimUvController.h"

class ObjectBase
{
public:
	ObjectBase() = default;
	virtual ~ObjectBase() = default;

	virtual void Update() = 0;
	virtual void Draw() = 0;

protected:
	struct Vertex
	{
		D3DXVECTOR2			   Pos;
		D3DXVECTOR2			   Rect;
		Lib::AnimUvController* pUvController;
		Lib::Vertex2D*		   pVertex;
	};

	/**
	 * Vertexの初期化
	 * @param[in] _pos 座標
	 * @param[in] _rect 矩形
	 * @param[in] _animFile 読み込むアニメーションファイルの名前
	 * @param[in] _animName 読み込むアニメーションの名前
	 * @param[out] _vertex 初期化するVertex構造体
	 */
	void InitVertex(
		const D3DXVECTOR2& _pos,
		const D3DXVECTOR2& _rect,
		LPCTSTR _animFile,
		LPCTSTR _animName, 
		Vertex* _vertex);

	/**
	 * Vertexの開放
	 * @param[in] 開放するvertex構造体
	 */
	void ReleaseVertex(Vertex* _vertex);

};


#endif
