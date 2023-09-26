#pragma once

#include "GameBaseScene.h"
#include "AbstractSceneFactory.h"

//シーン管理
class GameSceneManager final
{
private: //メンバ関数
	//コンストラクタ
	GameSceneManager() = default;
	//デストラクタ
	~GameSceneManager();

public: //メンバ関数
	//更新
	void Update();

	//描画
	void Draw();

	//シーンの解放
	void Destroy();

public:
	//インスタンスの作成
	static GameSceneManager* GetInstance();

private:
	//コピーコンストラクタを無効にする
	GameSceneManager(const GameSceneManager&) = delete;
	//代入演算子を無効にする
	GameSceneManager& operator=(const GameSceneManager&) = delete;

public: //メンバ関数
	//次シーン予約
	void ChangeScene(const std::string& sceneName);

	//シーンファクトリーのsetter
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

private: //メンバ変数
	//今のシーン(実行中シーン)
	GameBaseScene* scene_ = nullptr;
	//次シーン
	GameBaseScene* nextScene_ = nullptr;
	//シーンファクトリー（借りてくる）
	AbstractSceneFactory* sceneFactory_ = nullptr;
};

