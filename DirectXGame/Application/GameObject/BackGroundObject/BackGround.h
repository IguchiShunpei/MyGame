/**
* @file BackGround.h
* @brief 隕石の処理クラス
* @author イグチ_シュンペイ
*/

#pragma once

#include "Meteor.h"
#include "Stardust.h"
#include "LevelLoader.h"

#include <map>
#include <memory>

struct LevelData;

class BackGround
{
public:
	~BackGround();
	//初期化
	void Initialize();

	//更新
	void Update();

	//レベルロード
	void LoadLevel();

	//描画
	void Draw(ViewProjection*view);

private:

	//レベルデータ
	LevelData* backGroundStar_ = nullptr;
	LevelData* backGroundMeteor_ = nullptr;

	//// モデルデータコンテナ
	//std::map<std::string, std::unique_ptr <Model>> meteorModels_;
	//std::map<std::string, std::unique_ptr <Model>> stardustModels_;
	//// オブジェクト
	//std::vector<std::unique_ptr <Meteor>> meteorObjects_;
	//std::vector<std::unique_ptr <Stardust>> stardustObjects_;
	//std::unique_ptr <Stardust>backStardust_;
	//std::unique_ptr <Model>backModelStardust_;
	//std::unique_ptr <Meteor>backMeteor_;
	//std::unique_ptr <Model>backModelMeteor_;

	// モデルデータコンテナ
	std::map<std::string, Model*> meteorModels_;
	std::map<std::string, Model*> stardustModels_;
	// オブジェクト
	std::vector<Meteor*> meteorObjects_;
	std::vector<Stardust*> stardustObjects_;
	Stardust* backStardust_;
	Model* backModelStardust_;
	Meteor* backMeteor_;
	Model* backModelMeteor_;
};