/**
* @file UI.h
* @brief UI関連のクラス
* @author イグチ_シュンペイ
*/

#pragma once

#include "Sprite.h"
#include <memory>

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
	DirectXCommon* dxCommon_ = nullptr;

	//ロゴやUI
	std::unique_ptr <Sprite> sprite_;
	SpriteCommon spriteCommon_;
	Sprite arrowUp_;
	Sprite arrowDown_;
	Sprite arrowRight_;
	Sprite arrowLeft_;
	//hp
	Sprite hpBar_;
	Sprite hpFrame_;
	//スコア
	Sprite score_;

	//画面
	Sprite black_;
	Sprite red_;
	Sprite green_;

	//画面端とUIの距離
	float UiRange_;
	//UIの色(押されていないの下限)
	float uiColorNormal_;
	//UIの色uiColor_(押されているときの上限)
	float uiColorPush_;
	//UIの色(現在)
	float uiColor_[4];
	//UIのいろの変動値
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
};