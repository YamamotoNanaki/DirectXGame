#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

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

	worldTransform_.scale_ = { 5.0f,5.0f,5.0f };
	worldTransform_.rotation_ = { XM_PI / 4.0f,XM_PI / 4.0f,0.0f };
	worldTransform_.translation_ = { 10.0f,10.0f,10.0f };

	worldTransform_.Initialize();
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

	/*if (input_->TriggerKey(DIK_SPACE))
	{
		audio_->StopWave(voiceHandle_);
	}*/

	//value_++;
	string strDebug = string("scale:") + to_string(worldTransform_.scale_.x) + string(",")
		+ to_string(worldTransform_.scale_.y) + string(",") + to_string(worldTransform_.scale_.z);
	debugText_->Print(strDebug, 50, 50, 1.0f);
	strDebug = string("rotation:") + to_string(worldTransform_.rotation_.x) + string(",")
		+ to_string(worldTransform_.rotation_.y) + string(",") + to_string(worldTransform_.rotation_.z);
	debugText_->Print(strDebug, 50, 70, 1.0f);
	strDebug = string("translation:") + to_string(worldTransform_.translation_.x) + string(",")
		+ to_string(worldTransform_.translation_.y) + string(",") + to_string(worldTransform_.translation_.z);
	debugText_->Print(strDebug, 50, 90, 1.0f);

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
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

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
