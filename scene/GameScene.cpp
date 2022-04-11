#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;
using namespace std;

GameScene::GameScene() {}

GameScene::~GameScene() 
{
	delete sprite_;
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");
	sprite_ = Sprite::Create(textureHandle_, { 100,50 });
	model_ = Model::Create();

	random_device seed_gen;
	mt19937_64 engine(seed_gen());
	uniform_real_distribution<float>rotDist(0.0f, XM_2PI);
	uniform_real_distribution<float>posDist(-10.0f, 10.0f);

	for (size_t i = 0; i < _countof(worldTransform_); i++)
	{
		worldTransform_[i].scale_ = { 1,1,1 };
		worldTransform_[i].rotation_ = { rotDist(engine),rotDist(engine),rotDist(engine) };
		worldTransform_[i].translation_ = { posDist(engine),posDist(engine),posDist(engine) };
		worldTransform_[i].Initialize();
	}


	viewProjection_.Initialize();
	/*soundDataHandle_ = audio_->LoadWave("se_sad03.wav");
	audio_->PlayWave(soundDataHandle_);
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);*/
}

void GameScene::Update() 
{
	//スプライトの今の座標を取得
	XMFLOAT2 position = sprite_->GetPosition();
	//座標
	position.x += 2.0f;
	position.y += 1.0f;

	sprite_->SetPosition(position);

#pragma region 視点移動
	XMFLOAT3 move = { 0,0,0 };

	const float kEyeSpeed = 0.2f;

	if (input_->PushKey(DIK_W))	move = { 0,0,+kEyeSpeed };
	if (input_->PushKey(DIK_S))	move = { 0,0,-kEyeSpeed };

	viewProjection_.eye.x += move.x;
	viewProjection_.eye.y += move.y;
	viewProjection_.eye.z += move.z;

	viewProjection_.UpdateMatrix();

	debugText_->SetPos(50, 50);
	debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

#pragma endregion 視点移動

#pragma region 注視点移動

	const float kTargetSpeed = 0.2f;

	if (input_->PushKey(DIK_RIGHT))	move = { +kTargetSpeed,0,0 };
	if (input_->PushKey(DIK_LEFT))	move = { -kTargetSpeed,0,0 };

	viewProjection_.target.x += move.x;
	viewProjection_.target.y += move.y;
	viewProjection_.target.z += move.z;

	viewProjection_.UpdateMatrix();

	debugText_->SetPos(50, 70);
	debugText_->Printf("target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);

#pragma endregion 注視点移動

#pragma region 上方向回転処理

	const float kUpRotSpeed = 0.05f;

	if (input_->PushKey(DIK_SPACE))
	{
		viewAngle += kUpRotSpeed;
		viewAngle = fmodf(viewAngle, XM_2PI);
	}
	viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };

	viewProjection_.UpdateMatrix();

	debugText_->SetPos(50, 90);
	debugText_->Printf("up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);

#pragma endregion 上方向回転処理


	/*if (input_->TriggerKey(DIK_SPACE))
	{
		audio_->StopWave(voiceHandle_);
	}*/

	//value_++;
	/*string strDebug = string("scale:") + to_string(worldTransform_.scale_.x) + string(",")
		+ to_string(worldTransform_.scale_.y) + string(",") + to_string(worldTransform_.scale_.z);
	debugText_->Print(strDebug, 50, 50, 1.0f);
	strDebug = string("rotation:") + to_string(worldTransform_.rotation_.x) + string(",")
		+ to_string(worldTransform_.rotation_.y) + string(",") + to_string(worldTransform_.rotation_.z);
	debugText_->Print(strDebug, 50, 70, 1.0f);
	strDebug = string("translation:") + to_string(worldTransform_.translation_.x) + string(",")
		+ to_string(worldTransform_.translation_.y) + string(",") + to_string(worldTransform_.translation_.z);
	debugText_->Print(strDebug, 50, 90, 1.0f);*/

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	//sprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	for (size_t i = 0; i < _countof(worldTransform_); i++)
	{
		model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
