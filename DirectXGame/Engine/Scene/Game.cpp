#include "Game.h"

#include "SceneFactory.h"

void Game::Initialize()
{	
	// 基底クラスの初期化処理
	SIFrameWork::Initialize();

	// シーンファクトリを生成し、マネージャにセット
	sceneFactory_ = new SceneFactory();
	sceneManager_->SetSceneFactory(sceneFactory_);
	// シーンマネージャに最初のシーンをセット
	sceneManager_->ChangeScene("TITLE");
}