/**
 * @file   Map.h
 * @brief  Mapクラスのヘッダファイル
 * @author kotani
 */
#ifndef MAP_H
#define MAP_H
#include "../ObjectBase.h"
#define MAP_WIDTH 40
#define MAP_HEIGHT 23
#define CHIP_SIZE 32

class Map : public ObjectBase
{
public:
	Map();
	~Map();

	void Update() override;
	void Draw() override;

private:
	enum MAP_CHIP_TYPE
	{
		CHIP_NONE,   //!< 何も描画しない
		CHIP_GROUND, //!< 地面を描画する
		CHIP_DEATH   //!< 当たると倒される
	};
	void MapLoad();

	ObjectBase::Vertex m_Vertex;
	int m_MapChip[MAP_HEIGHT][MAP_WIDTH];
	int m_TextureIndex;
};


#endif
