/**
* @file CameraEditor.cpp
* @brief 簡単なカメラエディタ
* @author イグチ_シュンペイ
*/

#include "CameraEditor.h"

CameraEditor::~CameraEditor()
{

}

void CameraEditor::Initialize()
{
	input_ = SIEngine::Input::GetInstance();
	dxCommon_ = SIEngine::DirectXCommon::GetInstance();

	//天球
	sky_ = std::make_unique < SkyDome>();
	sky_->SkyDomeInitialize();
	skyModel_ = Model::LoadFromOBJ("editorSky");
	sky_->SetModel(skyModel_.get());
	sky_->SetScale(Vector3(50.0f, 50.0f, 50.0f));

	//player 
	player_ = std::make_unique <Player>();
	player_->PlayerInitialize();
	player_->worldTransform_.position_ = { 0.0f,0.0f,0.0f };
	player_->worldTransform_.UpdateMatrix();

	//カメラ
	viewProjection_ = std::make_unique <ViewProjection>();
	viewProjection_->Initialize();
	viewProjection_->SetTarget(player_->GetPosition());
	viewProjection_->SetEye(Vector3(0.0f, 20.0f, 30.0f));

	//UIの初期化
	ui_ = std::make_unique<UI>();
	ui_->UIInitialize();

	isToTitle_ = false;

	isTest_ = false;

	moveTime_ = 0.0f;
	movetimeMax_ = 90.0f;

	beforeEye_ = viewProjection_->eye_;
}

void CameraEditor::Update()
{
	cameraObjects_.remove_if([](std::unique_ptr <CameraObject>& cObject)
		{
			return cObject->GetIsDelete();
		});

	//天球
	sky_->Update();

	//黒フェードアウト
	ui_->FadeOut(ui_->Black);

	//テスト
	Test();

	//Tキーを押したとき
	if (input_->PushKey(DIK_C))
	{
		isToTitle_ = true;
	}

	//タイトルシーンへ
	ToTitle();

	//カメラ
	viewProjection_->UpdateMatrix();

}

void CameraEditor::Add()
{
	//Tキーを押したとき
	if (input_->PushKey(DIK_T))
	{

	}
}

void CameraEditor::Delete()
{
	//Dキーを押したとき
	if (input_->PushKey(DIK_D))
	{
		//全てのカメラオブジェクトの中で
		for (std::unique_ptr<CameraObject>& cObjects : cameraObjects_)
		{
			//選択フラグがtrueのものを削除
			if (cObjects->GetIsSelect() == true)
			{
				cObjects->SetIsDelete(true);
			}
		}
	}
}

void CameraEditor::Test()
{
	//全てのカメラオブジェクトの中で
	for (std::unique_ptr<CameraObject>& cObjects : cameraObjects_)
	{
		//選択フラグがtrueのものを削除
		if (cObjects->GetIsSelect() == true)
		{
			isTest_ = true;
			/*	Move(cObjects[Start]->GetPosition(), cObjects[End]->GetPosition());*/
		}
	}
}

void CameraEditor::Move(Vector3 start, Vector3 end)
{
	if (isTest_ == true)
	{
		if (moveTime_ <= movetimeMax_)
		{
			viewProjection_->eye_.x = start.x + (end.x - start.x) * MathFunc::easeOutSine(moveTime_ / movetimeMax_);
			viewProjection_->eye_.y = start.y + (end.y - start.y) * MathFunc::easeOutSine(moveTime_ / movetimeMax_);
			viewProjection_->eye_.z = start.z + (end.z - start.z) * MathFunc::easeOutSine(moveTime_ / movetimeMax_);
			moveTime_++;
		}
		else
		{
			viewProjection_->eye_ = beforeEye_;
			moveTime_ = 0.0f;
			isTest_ = false;
		}
	}
}

void CameraEditor::Draw()
{

	dxCommon_->PreDraw();

	Object3d::PreDraw(dxCommon_->GetCommandList());

	//天球
	sky_->Draw(viewProjection_.get());

	//自機
	player_->Draw(viewProjection_.get());

	//カメラオブジェクト
	for (std::unique_ptr<CameraObject>& cObjects : cameraObjects_)
	{
		cObjects->Draw(viewProjection_.get(), cObjects->GetAlpha(), cObjects->GetColor());
	}

	Object3d::PostDraw();

	//UI
	ui_->UIDraw();

	// 描画後処理
	dxCommon_->PostDraw();
}

void CameraEditor::Finalize()
{

}

void CameraEditor::OutTextFile()
{

}

void CameraEditor::ToTitle()
{
	if (isToTitle_ == true)
	{
		//黒フェードイン
		ui_->SetIsBlack(false);
		ui_->FadeIn();
		if (ui_->GetIsBlack() == true)
		{
			//TITLE（次シーン）を生成
			GameSceneManager::GetInstance()->ChangeScene("TITLE");
		}
	}
}
