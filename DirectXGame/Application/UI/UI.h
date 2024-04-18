/**
* @file UI.h
* @brief UI関連のクラス
* @author イグチ_シュンペイ
*/

#pragma once

#include "Sprite.h"
#include "ViewProjection.h"
#include "Player.h"
#include <memory>

#include <fstream>
#include <string.h>
#include <math.h>

class UI
{
public:
	//向き
	enum Direction
	{
		UP,
		DOWN,
		RIGHT,
		LEFT
	};

public:
	//色
	enum Color
	{
		Black,
		Red
	};

public:
	//初期化
	void UIInitialize();
	//登場モーション
	void UIInitMotion();
	//退場モーション
	void UIOutMotion();
	//更新
	void UIUpdate();
	//描画
	void UIDraw();
	//移動
	void UIMove();
	//色変化
	void UIColorChange();
	//hpダメージ処理
	void Damage(float t,float tMax,int hp,int hpMax);
	//画面のフェード処理(in)
	void FadeIn();
	//画面のフェード処理(out)
	void FadeOut(int color);
	//処理停止
	void Stop();
	//赤リセット
	void RedReset();

	//チュートリアル
	void StartTutorial();
	void EndTutorial(bool& isTutorial);

	//パワーアップ
	void PowerUp();

	//スコア
	void ScoreCalc(int score);

	//getter
	bool GetIsBlack() const { return isBlack_; };

	//setter
	void SetIsUp(bool isUp) { isUp_ = isUp; }
	void SetIsDown(bool isDown) { isDown_ = isDown; }
	void SetIsRight(bool isRight) { isRight_ = isRight; }
	void SetIsLeft(bool isLeft) { isLeft_ = isLeft; }
	void SetIsRed(bool isRed) { isRed_ = isRed; }
	void SetIsBlack(bool isBlack) { isBlack_ = isBlack; }

private:
	//DxCommon
	SIEngine::DirectXCommon* dxCommon_ = nullptr;

	//ロゴやUI
	std::unique_ptr <SIEngine::Sprite> sprite_;
	SIEngine::SpriteCommon spriteCommon_;
	SIEngine::Sprite arrowUp_;
	SIEngine::Sprite arrowDown_;
	SIEngine::Sprite arrowRight_;
	SIEngine::Sprite arrowLeft_;
	//hp
	SIEngine::Sprite hpBar_;
	SIEngine::Sprite hpFrame_;
	//スコア
	SIEngine::Sprite score_;
	//スコア数値
	SIEngine::Sprite onesPlace[10];
	SIEngine::Sprite tensPlace[10];
	SIEngine::Sprite hundredPlace[10];
	SIEngine::Sprite thousandPlace[10];
	SIEngine::Sprite tenthousandPlace[10];
	SIEngine::Sprite hundredthousandPlace[10];

	//チュートリアル
	SIEngine::Sprite move_;
	SIEngine::Sprite shot_;
	SIEngine::Sprite finish_;

	//パワーアップ
	SIEngine::Sprite powerUp_;

	//画面
	SIEngine::Sprite black_;
	SIEngine::Sprite red_;
	SIEngine::Sprite green_;

	//画面端とUIの距離
	float UiRange_;
	//UIの色(押されていないの下限)
	float uiColorNormal_;
	//UIの色uiColor_(押されているときの上限)
	float uiColorPush_;
	//UIの色(現在)
	float uiColor_[4];
	//UIの色の変動値
	float uiColorNum_;

	//タイトル
	float logoTime_;
	float logoY_;

	//黒画面フラグ
	bool isBlack_;
	//黒alpha値
	float blackAlpha_;
	//変動値
	float blackAlphaNum_;
	//最大値
	float blackAlphaNumMax_;
	//最小値
	float blackAlphaNumMin_;
	//赤alpha値
	float redAlpha_;
	//変動値
	float redAlphaNum_;
	//最大値
	float redAlphaNumMax_;
	//最小値
	float redAlphaNumMin_;
	//被ダメージフラグ
	bool isRed_;
	//ダメージ量
	float hpDamage_;
	//緑alpha値
	float greenAlpha_;

	//各方向フラグ
	bool isUp_;
	bool isDown_;
	bool isRight_;
	bool isLeft_;
	bool isNeutral_;
	bool isUIInit_;
	bool isUIOut_;

	//登場モーション範囲
	float UIInitRange_;
	//登場モーション時間
	float UIInitTime_;
	//登場モーション移動量
	float UIInitPos_;
	//退場モーション範囲
	float UIOutRange_;
	//退場モーション時間
	float UIOutTime_;
	//退場モーション移動量
	float UIOutPos_;

	//登場後モーション
	//フラグ
	bool isMove_;
	float UIMoveTime_;
	float UIMovePos_;
	float UIMoveRange_;

	//スコア
	int scores[6];

	//チュートリアル
	float tutorialTime_;
	float tutorialTimeMax_;

	bool isStart_;

	Vector2 tutorialScale_;
	Vector2 tutorialScaleMax_;
};