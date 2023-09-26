#include "GameSceneManager.h"

GameSceneManager::~GameSceneManager()
{
	// 最後のシーンの終了と解放
	if (scene_ != nullptr) {
		scene_->Finalize();
		delete scene_;
		scene_ = nullptr;
	}
}

GameSceneManager* GameSceneManager::GetInstance() 
{
	static GameSceneManager instance;

	return &instance;
}

void GameSceneManager::Update()
{
	// 予約シーンの予約があるなら
	if (nextScene_) {
		// 旧シーンの終了
		if (scene_) {
			scene_->Finalize();

			delete scene_;
		}

		// シーン切り替え
		scene_ = nextScene_;
		nextScene_ = nullptr;

		// シーンマネージャをセット
		scene_->SetSceneManager(this);

		// 次シーンを初期化する
		scene_->Initialize();
	}

	// 実行中シーンを更新する
	scene_->Update();
}

void GameSceneManager::Draw()
{
	// シーンの描画
	scene_->Draw();
}

void GameSceneManager::Destroy()
{
	if (scene_ != nullptr) {
		scene_->Finalize();
		delete scene_;
		scene_ = nullptr;
	}
}

void GameSceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	// 次シーン生成
	nextScene_ = sceneFactory_->CreateScene(sceneName);
}
