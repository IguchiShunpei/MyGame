/**
* @file LevelLoader.cpp
* @brief レベルデータ処理をまとめたクラス
* @author イグチ_シュンペイ
*/

#pragma once

#include <string>
#include <vector>
#include <DirectXMath.h>

// レベルデータ
struct LevelData {

	struct ObjectData {
		// 平行移動
		DirectX::XMVECTOR transform;
		// 回転
		DirectX::XMVECTOR rotation;
		// スケール
		DirectX::XMVECTOR scaling;
		// ファイル名
		std::string fileName;
	};

	// オブジェクト配列
	std::vector<ObjectData> objects;
};

// レベルデータのローダー
class LevelLoader {

public:// 定数
	// デフォルトの読み込みディレクトリ
	static const std::string kDefaultBaseDirectory;
	// ファイル拡張子
	static const std::string kExtension;

public:// メンバ関数
	
	//ファイルの読み込み
	static LevelData* LoadFile(const std::string& fileName);
};

