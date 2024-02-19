/**
* @file Camera.h
* @brief カメラ挙動のクラス
* @author イグチ_シュンペイ
*/

#include "ViewProjection.h"
#include "Input.h"
#include "DirectXCommon.h"

#include "UI.h"
#include "Player.h"

#include <memory>

class Camera
{
public:
	//初期化
	void Initialize();

	//更新
	void Update();

	//プレイヤー登場カメラワーク
	void PlayerInitCameraWork();

	//ボス登場カメラワーク
	void BossInitCameraWork();

	//clearカメラワーク
	void ToClearCameraWork();

	//gameOverカメラワーク
	void ToGameOverCameraWork();

	//カメラシェイク
	void CameraShake(float x, float y,int time);

	//カメラ基本移動
	void MoveCamera();

	//Getter
	ViewProjection* GetViewProjection() const { return viewProjection_.get(); }

	//Setter
	static void SetUI(UI* ui) { ui_ = ui; }
	static void SetPlayer(Player* player) { player_ = player; }


private:
	//カメラ
	std::unique_ptr<ViewProjection> viewProjection_;

	//入力
	SIEngine::Input* input_ = nullptr;
	//DxCommon
	SIEngine::DirectXCommon* dxCommon_ = nullptr;

	//借りてくるもの
	static UI* ui_;
	static Player* player_;

	//target変動地
	float changeTarget_;

	//基本eye
	Vector3 normalEyeNum_;
	//基本target
	Vector3 normalTargetNum_;
	//基本up
	Vector3 normalUpNum_;

};