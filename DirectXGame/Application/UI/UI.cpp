/**
* @file UI.cpp
* @brief UI関連のクラス
* @author イグチ_シュンペイ
*/

#include "UI.h"
#include "MathFunc.h"

void UI::GameUIInitialize()
{
	dxCommon_ = SIEngine::DirectXCommon::GetInstance();

	//UI入退場の変数
	UIInitPos_ = 0.0f;
	UIInitRange_ = 100.0f;
	UIInitTime_ = 0.0f;
	UIOutPos_ = 0.0f;
	UIOutRange_ = 100.0f;
	UIOutTime_ = 0.0f;
	UIMovePos_ = 0.0f;
	UIMoveRange_ = 5.0f;
	UIMoveTime_ = 0.0f;

	//ダメージ
	hpDamage_ = 0.0f;
	//画面端との距離
	UiRange_ = 30.0f;
	//押されていないときの色
	uiColorNormal_ = 0.5f;
	//押されているときの色
	uiColorPush_ = 3.0f;
	//現在の色
	for (int i = 0; i < 4; i++)
	{
		uiColor_[i] = uiColorNormal_;
	}

	//色の変動値
	uiColorNum_ = 0.2f;

	isUIInit_ = false;
	isUIOut_ = false;
	isNeutral_ = true;
	isUp_ = false;
	isDown_ = false;
	isRight_ = false;
	isLeft_ = false;

	sprite_ = std::make_unique<SIEngine::Sprite>();
	spriteCommon_ = sprite_->SpriteCommonCreate(dxCommon_->GetDevice());

	//上
	arrowUp_.Initialize(dxCommon_->GetDevice(), 0, Vector2(0.5f, 0.5f), false, false);
	arrowUp_.SetScale({ 201,50 });
	arrowUp_.SetPosition({ 640,-50,0 });
	arrowUp_.SpriteTransferVertexBuffer(arrowUp_, 0);
	arrowUp_.Update(arrowUp_, spriteCommon_);
	arrowUp_.LoadTexture(spriteCommon_, 0, L"Resources/2d/up.png", dxCommon_->GetDevice());

	//下
	arrowDown_.Initialize(dxCommon_->GetDevice(), 1, Vector2(0.5f, 0.5f), false, false);
	arrowDown_.SetScale({ 201,50 });
	arrowDown_.SetPosition({ 640,770,0 });
	arrowDown_.SpriteTransferVertexBuffer(arrowDown_, 1);
	arrowDown_.Update(arrowDown_, spriteCommon_);
	arrowDown_.LoadTexture(spriteCommon_, 1, L"Resources/2d/down.png", dxCommon_->GetDevice());

	//右
	arrowRight_.Initialize(dxCommon_->GetDevice(), 2, Vector2(0.5f, 0.5f), false, false);
	arrowRight_.SetScale({ 50,201 });
	arrowRight_.SetPosition({ 1330,360,0 });
	arrowRight_.SpriteTransferVertexBuffer(arrowRight_, 2);
	arrowRight_.Update(arrowRight_, spriteCommon_);
	arrowRight_.LoadTexture(spriteCommon_, 2, L"Resources/2d/right.png", dxCommon_->GetDevice());

	//左
	arrowLeft_.Initialize(dxCommon_->GetDevice(), 3, Vector2(0.5f, 0.5f), false, false);
	arrowLeft_.SetScale({ 50,201 });
	arrowLeft_.SetPosition({ -50,360,0 });
	arrowLeft_.SpriteTransferVertexBuffer(arrowLeft_, 3);
	arrowLeft_.Update(arrowLeft_, spriteCommon_);
	arrowLeft_.LoadTexture(spriteCommon_, 3, L"Resources/2d/left.png", dxCommon_->GetDevice());

	//黒
	isBlack_ = true;
	blackAlpha_ = 1.0f;
	blackAlphaNum_ = 0.02f;
	blackAlphaNumMax_ = 1.0f;
	blackAlphaNumMin_ = 0.0f;
	black_.Initialize(dxCommon_->GetDevice(), 4, Vector2(0.0f, 0.0f), false, false);
	black_.SetScale(Vector2(1280 * 1, 720 * 1));
	black_.SetPosition({ 0,0,0 });
	black_.SpriteTransferVertexBuffer(black_, 4);
	black_.SetAlpha(black_, blackAlpha_);
	black_.Update(black_, spriteCommon_);
	black_.LoadTexture(spriteCommon_, 4, L"Resources/2d/black.png", dxCommon_->GetDevice());

	//赤
	redAlpha_ = 0.5f;
	redAlphaNum_ = 0.02f;
	redAlphaNumMax_ = 0.5f;
	redAlphaNumMin_ = 0.0f;
	isRed_ = false;
	red_.Initialize(dxCommon_->GetDevice(), 5, Vector2(0.0f, 0.0f), false, false);
	red_.SetScale(Vector2(1280 * 1, 720 * 1));
	red_.SetPosition({ 0,0,0 });
	red_.SpriteTransferVertexBuffer(red_, 5);
	red_.SetAlpha(red_, redAlpha_);
	red_.Update(red_, spriteCommon_);
	red_.LoadTexture(spriteCommon_, 5, L"Resources/2d/red.png", dxCommon_->GetDevice());

	//hp関係
	hpBar_.Initialize(dxCommon_->GetDevice(), 6, Vector2(0.5f, 1.0f), false, false);
	hpBar_.SetScale({ 24,234 });
	hpBar_.SpriteTransferVertexBuffer(hpBar_, 6);
	hpBar_.Update(hpBar_, spriteCommon_);
	hpBar_.LoadTexture(spriteCommon_, 6, L"Resources/2d/hpBar.png", dxCommon_->GetDevice());
	//枠
	hpFrame_.Initialize(dxCommon_->GetDevice(), 7, Vector2(0.5f, 1.0f), false, false);
	hpFrame_.SetScale({ 30,240 });
	hpFrame_.SpriteTransferVertexBuffer(hpFrame_, 7);
	hpFrame_.Update(hpFrame_, spriteCommon_);
	hpFrame_.LoadTexture(spriteCommon_, 7, L"Resources/2d/hpFrame.png", dxCommon_->GetDevice());

	//緑
	greenAlpha_ = 0.1f;
	green_.Initialize(dxCommon_->GetDevice(), 8, Vector2(0.0f, 0.0f), false, false);
	green_.SetScale(Vector2(1280 * 1, 720 * 1));
	green_.SetPosition({ 0,0,0 });
	green_.SpriteTransferVertexBuffer(green_, 8);
	green_.SetAlpha(green_, greenAlpha_);
	green_.Update(green_, spriteCommon_);
	green_.LoadTexture(spriteCommon_, 8, L"Resources/2d/green.png", dxCommon_->GetDevice());

	//スコア
	score_.Initialize(dxCommon_->GetDevice(), 9, Vector2(0.0f, 0.0f), false, false);
	score_.SetScale({ 140,60 });
	score_.SetPosition({ 1100,0 - UIInitRange_,0 });
	score_.SpriteTransferVertexBuffer(score_, 9);
	score_.Update(score_, spriteCommon_);
	score_.LoadTexture(spriteCommon_, 9, L"Resources/2d/score.png", dxCommon_->GetDevice());

	//number
	const wchar_t* pathname = L"Resources/2d/numbers/";
	const wchar_t* extantion = L".png";
	wchar_t filename[16] = L"";

	for (int i = 0; i < 10; i++)
	{
		//1
		onesPlace[i].Initialize(dxCommon_->GetDevice(), 10 + i, Vector2(0.0f, 0.0f), false, false);
		int num = i;
		_itow_s(num, filename, 10);
		wchar_t path[128] = L"";
		wcscat_s(path, pathname);
		wcscat_s(path, filename);
		wcscat_s(path, extantion);

		onesPlace[i].SetScale({ 36.0f,72.0f });
		onesPlace[i].SetPosition({ 1180,0 - UIInitRange_,0 });
		onesPlace[i].SpriteTransferVertexBuffer(onesPlace[i], 10 + i);
		onesPlace[i].Update(onesPlace[i], spriteCommon_);
		onesPlace[i].LoadTexture(spriteCommon_, 10 + i, path, dxCommon_->GetDevice());
		//10
		tensPlace[i].Initialize(dxCommon_->GetDevice(), 20 + i, Vector2(0.0f, 0.0f), false, false);
		tensPlace[i].SetScale({ 36.0f,72.0f });
		tensPlace[i].SetPosition({ 1140,0 - UIInitRange_,0 });
		tensPlace[i].SpriteTransferVertexBuffer(tensPlace[i], 20 + i);
		tensPlace[i].Update(tensPlace[i], spriteCommon_);
		tensPlace[i].LoadTexture(spriteCommon_, 20 + i, path, dxCommon_->GetDevice());
		//100
		hundredPlace[i].Initialize(dxCommon_->GetDevice(), 30 + i, Vector2(0.0f, 0.0f), false, false);
		hundredPlace[i].SetScale({ 36.0f,72.0f });
		hundredPlace[i].SetPosition({ 1100,0 - UIInitRange_,0 });
		hundredPlace[i].SpriteTransferVertexBuffer(hundredPlace[i], 30 + i);
		hundredPlace[i].Update(hundredPlace[i], spriteCommon_);
		hundredPlace[i].LoadTexture(spriteCommon_, 30 + i, path, dxCommon_->GetDevice());
		//1000
		thousandPlace[i].Initialize(dxCommon_->GetDevice(), 40 + i, Vector2(0.0f, 0.0f), false, false);
		thousandPlace[i].SetScale({ 36.0f,72.0f });
		thousandPlace[i].SetPosition({ 1060,0 - UIInitRange_,0 });
		thousandPlace[i].SpriteTransferVertexBuffer(thousandPlace[i], 40 + i);
		thousandPlace[i].Update(thousandPlace[i], spriteCommon_);
		thousandPlace[i].LoadTexture(spriteCommon_, 40 + i, path, dxCommon_->GetDevice());
		//10000
		tenthousandPlace[i].Initialize(dxCommon_->GetDevice(), 50 + i, Vector2(0.0f, 0.0f), false, false);
		tenthousandPlace[i].SetScale({ 36.0f,72.0f });
		tenthousandPlace[i].SetPosition({ 1020,0 - UIInitRange_,0 });
		tenthousandPlace[i].SpriteTransferVertexBuffer(tenthousandPlace[i], 50 + i);
		tenthousandPlace[i].Update(tenthousandPlace[i], spriteCommon_);
		tenthousandPlace[i].LoadTexture(spriteCommon_, 50 + i, path, dxCommon_->GetDevice());
		//100000
		hundredthousandPlace[i].Initialize(dxCommon_->GetDevice(), 60 + i, Vector2(0.0f, 0.0f), false, false);
		hundredthousandPlace[i].SetScale({ 36.0f,72.0f });
		hundredthousandPlace[i].SetPosition({ 980,0 - UIInitRange_,0 });
		hundredthousandPlace[i].SpriteTransferVertexBuffer(hundredthousandPlace[i], 60 + i);
		hundredthousandPlace[i].Update(hundredthousandPlace[i], spriteCommon_);
		hundredthousandPlace[i].LoadTexture(spriteCommon_, 60 + i, path, dxCommon_->GetDevice());
	}

	tutorialTime_ = 0.0f;
	tutorialTimeMax_ = 30.0f;
	tutorialScale_ = { 0.0f,0.0f };
	tutorialScaleMax_ = { 256.0f,128.0f };
	isStart_ = false;

	//スコア
	move_.Initialize(dxCommon_->GetDevice(), 70, Vector2(0.5f, 0.5f), false, false);
	move_.SetScale(tutorialScale_);
	move_.SetPosition({ 250,150 ,0 });
	move_.SpriteTransferVertexBuffer(move_, 70);
	move_.Update(move_, spriteCommon_);
	move_.LoadTexture(spriteCommon_, 70, L"Resources/2d/MOVE.png", dxCommon_->GetDevice());

	shot_.Initialize(dxCommon_->GetDevice(), 71, Vector2(0.5f, 0.5f), false, false);
	shot_.SetScale(tutorialScale_);
	shot_.SetPosition({ 1030,150 ,0 });
	shot_.SpriteTransferVertexBuffer(shot_, 71);
	shot_.Update(shot_, spriteCommon_);
	shot_.LoadTexture(spriteCommon_, 71, L"Resources/2d/SHOT.png", dxCommon_->GetDevice());

	finish_.Initialize(dxCommon_->GetDevice(), 72, Vector2(0.5f, 0.5f), false, false);
	finish_.SetScale(tutorialScale_);
	finish_.SetPosition({ 250,360 ,0 });
	finish_.SpriteTransferVertexBuffer(finish_, 72);
	finish_.Update(finish_, spriteCommon_);
	finish_.LoadTexture(spriteCommon_, 72, L"Resources/2d/FINISH.png", dxCommon_->GetDevice());

	//各変数の初期化
	for (int i = 0; i < 6; i++) {
		scores[i] = 0;
	}
}

void UI::UIInitMotion()
{
	if (isUIInit_ == false)
	{
		if (UIInitPos_ < UIInitRange_)
		{
			UIInitPos_ = UIInitRange_ * MathFunc::easeOutSine(UIInitTime_ / 30.0f);
			UIInitTime_++;
			arrowUp_.SetPosition({ 640,UiRange_ - UIInitRange_ + UIInitPos_,0 });
			arrowDown_.SetPosition({ 640,720 - UiRange_ + UIInitRange_ - UIInitPos_,0 });
			arrowRight_.SetPosition({ 1280 - UiRange_ + UIInitRange_ - UIInitPos_,360,0 });
			arrowLeft_.SetPosition({ UiRange_ - UIInitRange_ + UIInitPos_,360,0 });
			hpBar_.SetPosition({ 50 - UIInitRange_ + UIInitPos_,697,0 });
			hpFrame_.SetPosition({ 50 - UIInitRange_ + UIInitPos_,700,0 });
			score_.SetPosition({ 820.0f, 680.0f - UiRange_ + UIInitRange_ - UIInitPos_, 0 });
			for (int i = 0; i < 10; i++)
			{
				onesPlace[i].SetPosition({ 1180,670 - UiRange_ + UIInitRange_ - UIInitPos_, 0 });
				tensPlace[i].SetPosition({ 1140,670 - UiRange_ + UIInitRange_ - UIInitPos_, 0 });
				hundredPlace[i].SetPosition({ 1100,670 - UiRange_ + UIInitRange_ - UIInitPos_, 0 });
				thousandPlace[i].SetPosition({ 1060,670 - UiRange_ + UIInitRange_ - UIInitPos_, 0 });
				tenthousandPlace[i].SetPosition({ 1020,670 - UiRange_ + UIInitRange_ - UIInitPos_, 0 });
				hundredthousandPlace[i].SetPosition({ 980,670 - UiRange_ + UIInitRange_ - UIInitPos_, 0 });

			}
		}
		else
		{
			isUIInit_ = true;
		}
	}
}

void UI::UIOutMotion()
{
	isNeutral_ = false;
	if (isUIOut_ == false)
	{
		if (UIOutPos_ < UIOutRange_)
		{
			UIOutPos_ = UIOutRange_ * MathFunc::easeOutSine(UIOutTime_ / 30.0f);
			UIOutTime_++;
			arrowUp_.SetPosition({ 640.0f,UiRange_ - UIOutPos_,0.0f });
			arrowDown_.SetPosition({ 640.0f,720.0f - UiRange_ + UIOutPos_,0.0f });
			arrowRight_.SetPosition({ 1280 - UiRange_ + UIOutPos_,360.0f,0.0f });
			arrowLeft_.SetPosition({ UiRange_ - UIOutPos_,360.0f,0.0f });
			hpBar_.SetPosition({ 0 - UIOutPos_,697,0 });
			hpFrame_.SetPosition({ 0 - UIOutPos_,700,0 });
			score_.SetPosition({ 820.0f,680.0f + UIOutPos_,0.0f });
			for (int i = 0; i < 10; i++)
			{
				onesPlace[i].SetPosition({ 1180,640 + UIOutPos_, 0 });
				tensPlace[i].SetPosition({ 1140,640 + UIOutPos_, 0 });
				hundredPlace[i].SetPosition({ 1100,640 + UIOutPos_, 0 });
				thousandPlace[i].SetPosition({ 1060,640 + UIOutPos_, 0 });
				tenthousandPlace[i].SetPosition({ 1020,640 + UIOutPos_, 0 });
				hundredthousandPlace[i].SetPosition({ 980,640 + UIOutPos_, 0 });

			}
		}
		else
		{
			UIOutPos_ = UIOutRange_;
			isUIOut_ = true;
			isUIInit_ = false;
		}
	}
}

void UI::GameUIUpdate()
{
	//登場後モーション
	UIMove();
	//色変化
	UIColorChange();

	//更新
	arrowUp_.Update(arrowUp_, spriteCommon_);
	arrowDown_.Update(arrowDown_, spriteCommon_);
	arrowRight_.Update(arrowRight_, spriteCommon_);
	arrowLeft_.Update(arrowLeft_, spriteCommon_);
	black_.Update(black_, spriteCommon_);
	red_.Update(red_, spriteCommon_);
	green_.Update(green_, spriteCommon_);
	hpBar_.Update(hpBar_, spriteCommon_);
	hpFrame_.Update(hpFrame_, spriteCommon_);
	score_.Update(score_, spriteCommon_);

	//スコア更新
	onesPlace[scores[0]].Update(onesPlace[scores[0]], spriteCommon_);
	tensPlace[scores[1]].Update(tensPlace[scores[1]], spriteCommon_);
	hundredPlace[scores[2]].Update(hundredPlace[scores[2]], spriteCommon_);
	thousandPlace[scores[3]].Update(thousandPlace[scores[3]], spriteCommon_);
	tenthousandPlace[scores[4]].Update(tenthousandPlace[scores[4]], spriteCommon_);
	hundredthousandPlace[scores[5]].Update(hundredthousandPlace[scores[5]], spriteCommon_);

	//チュートリアル
	move_.Update(move_, spriteCommon_);
	shot_.Update(shot_, spriteCommon_);
	finish_.Update(finish_, spriteCommon_);
}

void UI::GameUIDraw()
{
	//描画前処理
	SIEngine::Sprite::PreDraw(dxCommon_->GetCommandList(), spriteCommon_);

	//移動時のUI切り替え
	arrowUp_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	arrowDown_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	arrowRight_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	arrowLeft_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());

	//画面系
	if (isRed_ == true)
	{
		red_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	}

	hpFrame_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	hpBar_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	black_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());

	score_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());

	//スコア
	onesPlace[scores[0]].Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	tensPlace[scores[1]].Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	hundredPlace[scores[2]].Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	thousandPlace[scores[3]].Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	tenthousandPlace[scores[4]].Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	hundredthousandPlace[scores[5]].Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());

	//チュートリアル
	move_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	shot_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	finish_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
}

void UI::UIMove()
{
	if (isUIInit_ == true && isNeutral_ == true)
	{
		StartTutorial();
		if (isMove_ == true)
		{
			UIMovePos_ = UIMoveRange_ * MathFunc::easeOutSine(UIMoveTime_ / 30.0f);
			UIMoveTime_++;
			if (UIMoveTime_ >= 60.0f)
			{
				isMove_ = false;
				UIMoveTime_ = 0.0f;
			}
		}
		else
		{
			UIMovePos_ = UIMoveRange_ * -MathFunc::easeOutSine(UIMoveTime_ / 30.0f);
			UIMoveTime_++;
			if (UIMoveTime_ >= 60.0f)
			{
				isMove_ = true;
				UIMoveTime_ = 0.0f;
			}
		}
		arrowUp_.SetPosition({ 640,UiRange_ - UIMovePos_,0 });
		arrowDown_.SetPosition({ 640,720 - UiRange_ + UIMovePos_,0 });
		arrowRight_.SetPosition({ 1280 - UiRange_ + UIMovePos_,360,0 });
		arrowLeft_.SetPosition({ UiRange_ - UIMovePos_ ,360,0 });
	}
}

void UI::UIColorChange()
{
	//キーを押しているかで色を変化させる
	if (isUp_ == true)
	{
		if (uiColor_[UP] < uiColorPush_)
		{
			uiColor_[UP] += uiColorNum_;
		}
	}
	else
	{
		if (uiColor_[UP] > uiColorNormal_)
		{
			uiColor_[UP] -= uiColorNum_;
		}
	}
	if (isDown_ == true)
	{
		if (uiColor_[DOWN] < uiColorPush_)
		{
			uiColor_[DOWN] += uiColorNum_;
		}
	}
	else
	{
		if (uiColor_[DOWN] > uiColorNormal_)
		{
			uiColor_[DOWN] -= uiColorNum_;
		}
	}
	if (isRight_ == true)
	{
		if (uiColor_[RIGHT] < uiColorPush_)
		{
			uiColor_[RIGHT] += uiColorNum_;
		}
	}
	else
	{
		if (uiColor_[RIGHT] > uiColorNormal_)
		{
			uiColor_[RIGHT] -= uiColorNum_;
		}
	}
	if (isLeft_ == true)
	{
		if (uiColor_[LEFT] < uiColorPush_)
		{
			uiColor_[LEFT] += uiColorNum_;
		}
	}
	else
	{
		if (uiColor_[LEFT] > uiColorNormal_)
		{
			uiColor_[LEFT] -= uiColorNum_;
		}
	}
	//色をセット
	arrowUp_.SetColor(arrowUp_, { uiColor_[UP],uiColor_[UP] ,uiColor_[UP] ,1.0f });
	arrowDown_.SetColor(arrowDown_, { uiColor_[DOWN],uiColor_[DOWN] ,uiColor_[DOWN] ,1.0f });
	arrowRight_.SetColor(arrowRight_, { uiColor_[RIGHT],uiColor_[RIGHT] ,uiColor_[RIGHT] ,1.0f });
	arrowLeft_.SetColor(arrowLeft_, { uiColor_[LEFT],uiColor_[LEFT] ,uiColor_[LEFT] ,1.0f });
}

void UI::Damage(float t, float tMax, int hp, int hpMax)
{
	//ダメージのイージング処理
	hpDamage_ = 0.0f + (1.0f * MathFunc::easeOutSine((float)t / (float)tMax));
	//hpのスケールに残りhpを反映
	hpBar_.SetScale({ 24.0f,234.0f * ((((float)hp + 1.0f) - hpDamage_) / (float)hpMax) });
	hpBar_.SpriteTransferVertexBuffer(hpBar_, 6);
}

void UI::FadeIn()
{
	if (isBlack_ == false)
	{
		//黒フェードイン
		if (blackAlpha_ < blackAlphaNumMax_)
		{
			blackAlpha_ += blackAlphaNum_;
			black_.SetAlpha(black_, blackAlpha_);
		}
		else
		{
			blackAlpha_ = blackAlphaNumMax_;
			isBlack_ = true;
		}
		black_.Update(black_, spriteCommon_);
	}
}

void UI::FadeOut(int color)
{
	switch (color)
	{
	case Black:
		if (isBlack_ == true)
		{
			//黒フェードアウト
			if (blackAlpha_ > blackAlphaNumMin_)
			{
				blackAlpha_ -= blackAlphaNum_;
				black_.SetAlpha(black_, blackAlpha_);
			}
			else
			{
				blackAlpha_ = blackAlphaNumMin_;
			}
			black_.Update(black_, spriteCommon_);
		}
		break;
	case Red:
		if (isRed_ == true)
		{
			//赤フェードアウト
			if (redAlpha_ > redAlphaNumMin_)
			{
				redAlpha_ -= redAlphaNum_;
				red_.SetAlpha(red_, redAlpha_);
			}
			red_.Update(red_, spriteCommon_);
		}
		break;
	}
}

void UI::Stop()
{
	isUp_ = false;
	isDown_ = false;
	isRight_ = false;
	isLeft_ = false;
}

void UI::RedReset()
{
	red_.SetAlpha(red_, redAlphaNumMax_);
}

void UI::StartTutorial()
{
	if (isStart_ == false)
	{
		if (tutorialTime_ <= tutorialTimeMax_)
		{
			tutorialScale_.x = (tutorialScaleMax_.x * MathFunc::easeOutBack(tutorialTime_ / tutorialTimeMax_));
			tutorialScale_.y = (tutorialScaleMax_.y * MathFunc::easeOutBack(tutorialTime_ / tutorialTimeMax_));
			tutorialTime_++;
		}
		else
		{
			tutorialTime_ = 0.0f;
			isStart_ = true;
		}
		move_.SetScale(tutorialScale_);
		shot_.SetScale(tutorialScale_);
		finish_.SetScale(tutorialScale_);
		move_.SpriteTransferVertexBuffer(move_, 70);
		shot_.SpriteTransferVertexBuffer(shot_, 71);
		finish_.SpriteTransferVertexBuffer(finish_, 72);
	}
}

void UI::EndTutorial(bool& isTutorial)
{
	if (isTutorial == true && isStart_ == true)
	{
		if (tutorialTime_ <= tutorialTimeMax_)
		{
			tutorialScale_.x = tutorialScaleMax_.x - (tutorialScaleMax_.x * MathFunc::easeInBack(tutorialTime_ / tutorialTimeMax_));
			tutorialScale_.y = tutorialScaleMax_.y - (tutorialScaleMax_.y * MathFunc::easeInBack(tutorialTime_ / tutorialTimeMax_));
			tutorialTime_++;
		}
		else
		{
			isTutorial = false;
			tutorialTime_ = 0.0f;
		}
		move_.SetScale(tutorialScale_);
		shot_.SetScale(tutorialScale_);
		finish_.SetScale(tutorialScale_);
		move_.SpriteTransferVertexBuffer(move_, 70);
		shot_.SpriteTransferVertexBuffer(shot_, 71);
		finish_.SpriteTransferVertexBuffer(finish_, 72);
	}
}

void UI::PowerUp()
{

}

void UI::ScoreCalc(int score)
{
	if (score >= 100000) {
		scores[5] = score / 100000;
		score -= scores[5] * 100000;
		scores[4] = score / 10000;
		score -= scores[4] * 10000;
		scores[3] = score / 1000;
		score -= scores[3] * 1000;
		scores[2] = score / 100;
		score -= scores[2] * 100;
		scores[1] = score / 10;
		score -= scores[1] * 10;
		scores[0] = score;
	}
	else if (score >= 10000) {
		scores[5] = 0;
		scores[4] = score / 10000;
		score -= scores[4] * 10000;
		scores[3] = score / 1000;
		score -= scores[3] * 1000;
		scores[2] = score / 100;
		score -= scores[2] * 100;
		scores[1] = score / 10;
		score -= scores[1] * 10;
		scores[0] = score;
	}
	else if (score >= 1000) {
		scores[5] = 0;
		scores[4] = 0;
		scores[3] = score / 1000;
		score -= scores[3] * 1000;
		scores[2] = score / 100;
		score -= scores[2] * 100;
		scores[1] = score / 10;
		score -= scores[1] * 10;
		scores[0] = score;
	}
	else if (score >= 100) {
		scores[5] = 0;
		scores[4] = 0;
		scores[3] = 0;
		scores[2] = score / 100;
		score -= scores[2] * 100;
		scores[1] = score / 10;
		score -= scores[1] * 10;
		scores[0] = score;
	}
	else if (score >= 10) {
		scores[1] = score / 10;
		score -= scores[1] * 10;
		scores[0] = score;
		scores[2] = 0;
		scores[3] = 0;
		scores[4] = 0;
		scores[5] = 0;
	}
	else {
		scores[5] = 0;
		scores[4] = 0;
		scores[3] = 0;
		scores[2] = 0;
		scores[1] = 0;
		scores[0] = score;
	}
}
