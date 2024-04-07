#include "BackGround.h"

BackGround::~BackGround()
{

}

void BackGround::Initialize()
{
	//レベルデータのロード
	//背景オブジェクトデータの読み込み
	backGroundStar_ = LevelLoader::LoadFile("stardust");
	backGroundMeteor_ = LevelLoader::LoadFile("meteor");
	LoadLevel();
}

void BackGround::Update()
{
	for (auto& object : stardustObjects_)
	{
		object->StardustUpdate();
	}
	for (auto& object : meteorObjects_)
	{
		object->MeteorUpdate();
	}
}

void BackGround::LoadLevel()
{
	// レベルデータからオブジェクトを生成、配置
	//星屑
	for (auto& objectData : backGroundStar_->objects) {

		//モデル読み込み
		// モデルを指定して3Dオブジェクトを生成
		std::unique_ptr<Stardust> backStardust_ = std::make_unique<Stardust>();
		backStardust_->StardustInitialize();
		// 座標
		Vector3 pos;
		//データの値を代入
		pos.x = objectData.translation.m128_f32[0];
		pos.y = objectData.translation.m128_f32[1];
		pos.z = objectData.translation.m128_f32[2];
		//newObjectにセット
		backStardust_->SetPosition(pos);
		//初期Y座標を保存しておく
		backStardust_->SetBeforeY(pos.y);

		// 回転角
		Vector3 rot;
		//データの値を代入
		rot.x = objectData.rotation.m128_f32[0];
		rot.y = objectData.rotation.m128_f32[1];
		rot.z = objectData.rotation.m128_f32[2];
		//newObjectにセット
		backStardust_->SetRotation(rot);

		//色と大きさをセット
		backStardust_->SetColor();
		backStardust_->SetSize();

		// 配列に登録
		stardustObjects_.push_back(move(backStardust_));
	}
	//隕石
	for (auto& objectData : backGroundMeteor_->objects) {
		// モデルを指定して3Dオブジェクトを生成
		std::unique_ptr<Meteor> backMeteor_ = std::make_unique<Meteor>();
		backMeteor_->SetIsBack(true);
		backMeteor_->MeteorInitialize();
		// 座標
		Vector3 pos;
		//データの値を代入
		pos.x = objectData.translation.m128_f32[0];
		pos.y = objectData.translation.m128_f32[1];
		pos.z = objectData.translation.m128_f32[2];
		//newObjectにセット
		backMeteor_->SetPosition(pos);

		// 回転角
		Vector3 rot;
		//データの値を代入
		rot.x = objectData.rotation.m128_f32[0];
		rot.y = objectData.rotation.m128_f32[1];
		rot.z = objectData.rotation.m128_f32[2];
		//newObjectにセット
		backMeteor_->SetRotation(rot);

		// 大きさ
		Vector3 scale;
		//データの値を代入
		scale.x = objectData.scaling.m128_f32[0];
		scale.y = objectData.scaling.m128_f32[1];
		scale.z = objectData.scaling.m128_f32[2];
		//newObjectにセット
		backMeteor_->SetScale(scale);

		backMeteor_->SetIsBack(true);
		backMeteor_->SetAlpha(1.0f);
		backMeteor_->SetSpeed(0.1f, 1.0f);

		// 配列に登録
		meteorObjects_.push_back(move(backMeteor_));
	}
}

void BackGround::Draw(ViewProjection* view)
{
	//星屑
	for (auto& object : stardustObjects_) {
		object->Draw(view, 1.0f, object->GetColor());
	}
	//隕石
	for (auto& object : meteorObjects_) {
		object->Draw(view, object->GetAlpha(), object->GetColor());
	}

}
