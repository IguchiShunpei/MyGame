/**
* @file SceneFactory.cpp
* @brief 次のシーンを生成するクラス
* @author イグチ_シュンペイ
*/

#pragma once

#include "AbstractSceneFactory.h"
#include "GameTitleScene.h"
#include "GamePlayScene.h"
#include "GameClearScene.h"
#include "GameOverScene.h"

//このゲーム用のシーン工場
class SceneFactory : public AbstractSceneFactory
{
public:

	//シーン生成
	GameBaseScene* CreateScene(const std::string& sceneName) override;
};

