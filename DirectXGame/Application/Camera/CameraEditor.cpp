/**
* @file CameraEditor.cpp
* @brief 簡単なカメラエディタ
* @author イグチ_シュンペイ
*/

#include "CameraEditor.h"
#include <imgui.h>

CameraEditor::~CameraEditor()
{

}

void CameraEditor::Initialize()
{
	input_ = SIEngine::Input::GetInstance();
	dxCommon_ = SIEngine::DirectXCommon::GetInstance();
	imGuiManager_ = SIEngine::ImGuiManager::GetInstance();

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

	//カメラオブジェクト
	//オブジェクトの生成
	cameraObject_ = std::make_unique<CameraObject>();
	//オブジェクトの初期化
	cameraObject_->CObjectInitialize();
	//登録
	cameraObjects_.push_back(std::move(cameraObject_));

	//UIの初期化
	ui_ = std::make_unique<UI>();
	ui_->GameUIInitialize();

	//各変数の初期化
	isToTitle_ = false;

	isTest_ = false;

	moveTime_ = 0.0f;
	movetimeMax_ = 90.0f;

	beforeEye_ = viewProjection_->eye_;

	eyeZ_ = 40.0f;

	angle_ = 0.0f;
	angleNum_ = 1.0f;

	eyeLim_.y = 20.0f;
	eyeLim_.z = 60.0f;

	r_ = 0.0f;

	posLimit_ = 25.0f;
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

	TurnCamera();

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

	//デバックImGui
	imGuiManager_->Begin();

	ImGui::Text("CameraEditor");

	////カメラ追加
	//if (ImGui::Button("SetNewCameraObject"))
	//{
	//	Add();
	//}

	ImGui::Text("Camera");

	ImGui::SliderFloat("Y", &viewProjection_->eye_.y, -eyeLim_.y, eyeLim_.y);
	ImGui::SliderFloat("Z", &eyeZ_, 20.0f, eyeLim_.z);
	ImGui::SliderFloat("Angle", &angle_, -180.0f, 180.0f);


	ImGui::Text("CameraObject");
	//全てのカメラオブジェクトの中で
	for (std::unique_ptr<CameraObject>& cObjects : cameraObjects_)
	{
		//開始点の座標
		ImGui::Text("Start");
		ImGui::SliderFloat("Start_Position_X", &cObjects->GetCameraStart()->worldTransform_.position_.x, -posLimit_, posLimit_);
		ImGui::SliderFloat("Start_Position_Y", &cObjects->GetCameraStart()->worldTransform_.position_.y, -posLimit_, posLimit_);
		ImGui::SliderFloat("Start_Position_Z", &cObjects->GetCameraStart()->worldTransform_.position_.z, -posLimit_, posLimit_);
		//終着点の座標
		ImGui::Text("End");
		ImGui::SliderFloat("End_Position_X", &cObjects->GetCameraEnd()->worldTransform_.position_.x, -posLimit_, posLimit_);
		ImGui::SliderFloat("End_Position_Y", &cObjects->GetCameraEnd()->worldTransform_.position_.y, -posLimit_, posLimit_);
		ImGui::SliderFloat("End_Position_Z", &cObjects->GetCameraEnd()->worldTransform_.position_.z, -posLimit_, posLimit_);
	}

	//カメラ位置リセット
	if (ImGui::Button("ResetCamera"))
	{
		ResetCamera();
	}

	////カメラ削除
	//if (ImGui::Button("DeleteCameraObject"))
	//{
	//	Delete();
	//}

	//テスト
	if (ImGui::Button("Test"))
	{
		isTest_ = true;
	}

	Test();


	imGuiManager_->End();

	//全てのカメラオブジェクトの更新
	for (std::unique_ptr<CameraObject>& cObjects : cameraObjects_)
	{
		cObjects->CObjectUpdate();
	}
}

void CameraEditor::Add()
{
	//オブジェクトの生成
	std::unique_ptr<CameraObject> newObject = std::make_unique<CameraObject>();
	//オブジェクトの初期化
	newObject->CObjectInitialize();
	// 座標データに追加
	newObject->CObjectUpdate();
	//登録
	cameraObjects_.push_back(std::move(newObject));
}

void CameraEditor::ResetCamera()
{
	angle_ = 0.0f;
	viewProjection_->eye_.y = 0.0f;
	eyeZ_ = 40.0f;
}

void CameraEditor::Delete()
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

void CameraEditor::Test()
{
	//全てのカメラオブジェクトの中で
	for (std::unique_ptr<CameraObject>& cObjects : cameraObjects_)
	{
		if (isTest_ == true)
		{
			Move(cObjects->GetCameraStart()->GetPosition(), cObjects->GetCameraEnd()->GetPosition());
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
	if (isTest_ == false)
	{
		for (std::unique_ptr<CameraObject>& cObjects : cameraObjects_)
		{
			cObjects->CObjectDraw(viewProjection_.get());
		}
	}

	Object3d::PostDraw();

	//UI
	ui_->GameUIDraw();

	// ImGui描画
	imGuiManager_->Draw(dxCommon_);

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

void CameraEditor::TurnCamera()
{
	r_ = angle_ * MathFunc::PI() / MathFunc::Degree180();

	viewProjection_->eye_.x = player_->worldTransform_.position_.x + sinf(r_) * eyeZ_;
	viewProjection_->eye_.z = player_->worldTransform_.position_.z + cosf(r_) * eyeZ_;
}
