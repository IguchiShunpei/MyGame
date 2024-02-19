/**
* @file UI.cpp
* @brief UI関連のクラス
* @author イグチ_シュンペイ
*/

#include "UI.h"
#include "MathFunc.h"

void UI::UIInitialize()
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
	score_.SetPosition({ 50,0 - UIInitRange_,0 });
	score_.SetColor(score_, { uiColorPush_,uiColorPush_ ,uiColorPush_ ,1.0f });
	score_.SpriteTransferVertexBuffer(score_, 9);
	score_.Update(score_, spriteCommon_);
	score_.LoadTexture(spriteCommon_, 9, L"Resources/2d/score.png", dxCommon_->GetDevice());

	//レティクル
	reticle_.Initialize(dxCommon_->GetDevice(), 10, Vector2(0.5f, 0.5f), false, false);
	reticle_.SetScale({ 100,100 });
	reticle_.SetPosition({ 0,0,0 });
	reticle_.SetColor(reticle_, { uiColorPush_,uiColorPush_ ,uiColorPush_ ,1.0f });
	reticle_.SetAlpha(reticle_, reticleAlpha_);
	reticle_.SpriteTransferVertexBuffer(reticle_, 10);
	reticle_.Update(reticle_, spriteCommon_);
	reticle_.LoadTexture(spriteCommon_, 10, L"Resources/2d/reticle.png", dxCommon_->GetDevice());

	//レティクル関係変数初期化
	reticleWorldPos_ = { 0.0f,0.0f,0.0f };
	matViewport_.identity();
	matViewprojectionViewport_.identity();

	//レティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	distancePlayerToReticle_ = 80.0f;
	reticleAlpha_ = 0.0f;
	reticleAlphaNum_ = 0.1f;
	reticleAlphaNumMax_ = 1.0f;
	reticleAlphaNumMin_ = 0.0f;
	isInvicibleReticle_ = true;
}

void UI::UIInitMotion()
{
	if (isUIInit_ == false)
	{
		isInvicibleReticle_ = true;
		UIInitPos_ = UIInitRange_ * MathFunc::easeOutSine(UIInitTime_ / 30.0f);
		UIInitTime_++;
		arrowUp_.SetPosition({ 640,UiRange_ - UIInitRange_ + UIInitPos_,0 });
		arrowDown_.SetPosition({ 640,720 - UiRange_ + UIInitRange_ - UIInitPos_,0 });
		arrowRight_.SetPosition({ 1280 - UiRange_ + UIInitRange_ - UIInitPos_,360,0 });
		arrowLeft_.SetPosition({ UiRange_ - UIInitRange_ + UIInitPos_,360,0 });
		hpBar_.SetPosition({ 50 - UIInitRange_ + UIInitPos_,697,0 });
		hpFrame_.SetPosition({ 50 - UIInitRange_ + UIInitPos_,700,0 });
		score_.SetPosition({ 30, 10 - UIInitRange_ + UIInitPos_, 0 });
	}
	if (UIInitPos_ >= UIInitRange_)
	{
		isUIInit_ = true;
	}
}

void UI::UIOutMotion()
{
	isNeutral_ = false;
	isInvicibleReticle_ = false;
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
			score_.SetPosition({ 30.0f,10.0f - UIOutPos_,0.0f });
		}
		else
		{
			UIOutPos_ = UIOutRange_;
			isUIOut_ = true;
		}
	}
}

void UI::UIUpdate()
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
}

void UI::UIDraw()
{
	//描画前処理
	SIEngine::Sprite::PreDraw(dxCommon_->GetCommandList(), spriteCommon_);

	//移動時のUI切り替え
	arrowUp_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	arrowDown_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	arrowRight_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	arrowLeft_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());

	//画面系
	//green_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	if (isRed_ == true)
	{
		red_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	}

	hpFrame_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	hpBar_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	black_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
	reticle_.Draw(dxCommon_->GetCommandList(), spriteCommon_, dxCommon_->GetDevice());
}

void UI::UIMove()
{
	if (isUIInit_ == true && isNeutral_ == true)
	{
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
	red_.SetAlpha(red_,redAlphaNumMax_);
}

void UI::ReticleUpdate(ViewProjection* view, Player* player)
{
	if (isInvicibleReticle_ == true)
	{
		if (reticleAlpha_ < reticleAlphaNumMax_)
		{
			reticleAlpha_ += reticleAlphaNum_;
			reticle_.SetAlpha(reticle_, reticleAlpha_);
		}
	}
	else
	{
		if (reticleAlpha_ > reticleAlphaNumMin_)
		{
			reticleAlpha_ -= reticleAlphaNum_;
			reticle_.SetAlpha(reticle_, reticleAlpha_);
		}
	}
	//自機のワールド座標から3Dレティクルのワールド座標を計算
	{
		//自機から3Dレティクルへのオフセット(Z+向き)
		Vector3 offSet = { 0,0,1.0f };
		//自機のワールド行列の回転を反映
		offSet = MatVector(offSet, player->worldTransform_.matWorld_);
		//長さを整える
		offSet = offSet.normalize() * distancePlayerToReticle_;
		//3Dレティクルの座標を設定
		worldTransform3DReticle_.position_ = Vector3::AddVector3(player->GetWorldPosition(), offSet);
		worldTransform3DReticle_.UpdateMatrix();
	}
	//3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	{
		// 3Dレティクルのワールド行列から,ワールド座標を取得
		reticleWorldPos_.x = worldTransform3DReticle_.matWorld_.m[3][0];
		reticleWorldPos_.y = worldTransform3DReticle_.matWorld_.m[3][1];
		reticleWorldPos_.z = worldTransform3DReticle_.matWorld_.m[3][2];

		//ビューポート行列
		matViewport_ = SetViewport(Vector3(0, 0, 0));

		//ビュー行列とプロジェクション行列,ビューポート行列を合成する
		matViewprojectionViewport_ =
			view->matView_ * view->matProjection_ * matViewport_;
		//ワールド→スクリーン座標変換
		reticleWorldPos_ = Transform(reticleWorldPos_, matViewprojectionViewport_);
		//座標設定
		reticle_.SetPosition(Vector3(reticleWorldPos_.x, reticleWorldPos_.y, 0.0f));
	}
	reticle_.Update(reticle_, spriteCommon_);
}

Vector3 UI::MatVector(Vector3 v, Matrix4 mat)
{
	Vector3 pos;
	pos.x = -mat.m[0][0] * v.x + -mat.m[0][1] * v.y + -mat.m[0][2] * v.z + mat.m[0][3] * 1;
	pos.y = -mat.m[1][0] * v.x + -mat.m[1][1] * v.y + -mat.m[1][2] * v.z + mat.m[1][3] * 1;
	pos.z = mat.m[2][0] * v.x + mat.m[2][1] * v.y + mat.m[2][2] * v.z + mat.m[2][3] * 1;

	return pos;
}

Matrix4 UI::SetViewport(const Vector3& v)
{
	//単位行列の設定
	Matrix4 matViewport = Matrix4::identity();
	matViewport.m[0][0] = 1280.0f / 2.0f;
	matViewport.m[1][1] = -720.0f / 2.0f;
	matViewport.m[3][0] = (1280.0f / 2.0f) + v.x;
	matViewport.m[3][1] = (720.0f / 2.0f) + v.y;
	return matViewport;
}

Vector3 UI::Transform(const Vector3& v, const Matrix4& m)
{
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	Vector3 result
	{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w
	};

	return result;
}
