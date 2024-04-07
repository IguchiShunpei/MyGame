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

	// オブジェクト
	std::vector<std::unique_ptr <Meteor>> meteorObjects_;
	std::vector<std::unique_ptr <Stardust>> stardustObjects_;
};