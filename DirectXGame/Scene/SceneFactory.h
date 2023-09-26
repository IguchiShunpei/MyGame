#pragma once

#include "AbstractSceneFactory.h"
#include "GameTitleScene.h"
#include "GamePlayScene.h"
#include "GameClearScene.h"
#include "GameOverScene.h"

//���̃Q�[���p�̃V�[���H��
class SceneFactory : public AbstractSceneFactory
{
public:

	//�V�[������
	GameBaseScene* CreateScene(const std::string& sceneName) override;
};

