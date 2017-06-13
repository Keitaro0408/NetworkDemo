#ifndef MATCHINGSCENE_H
#define MATCHINGSCENE_H
#include "../SceneBase.h"

class MatchingScene : public SceneBase
{
public:
	MatchingScene();
	~MatchingScene();

	SceneID Update() override;
	void Draw() override;

};


#endif
