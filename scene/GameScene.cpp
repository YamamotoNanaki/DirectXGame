#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;
using namespace std;


GameScene::GameScene() {}

GameScene::~GameScene() 
{
	delete model_;
	delete sprite_;
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

	worldTransform_[1].translation_ = { 0,0,-10 };
	worldTransform_[1].parent_ = &worldTransform_[0];
	worldTransform_[1].scale_ = { 0.5,0.5,0.5 };
	for (int i = 2; i < _countof(worldTransform_); i++)
	{
		worldTransform_[i].translation_ = { 0,-4.5,-15.0f * i + 180 };
	}

	for (int i = 0; i < _countof(worldTransform_); i++)
	{
		worldTransform_[i].Initialize();
	}

	viewProjection_.eye.y = 15;

	viewProjection_.Initialize();

}

void GameScene::Update() 
{
	XMFLOAT3 front = { 0,0,0 };
	XMFLOAT3 move = { 0,0,0 };
	float rota = 0;

#pragma region キャラクター移動処理

	const float kCharacterSpeed = 0.2f;
	const float kRotaSpeed = 0.01;

	if (input_->PushKey(DIK_RIGHT))
	{
		rota += kRotaSpeed;
	}
	if (input_->PushKey(DIK_LEFT))
	{
		rota -= kRotaSpeed;
	}
	worldTransform_[PartId::Root].rotation_.y += rota;

	front = { sinf(worldTransform_[PartId::Root].rotation_.y),
		0, cosf(worldTransform_[PartId::Root].rotation_.y) };

	debugText_->SetPos(50, 50);
	debugText_->Printf("front(x:%f,y:%f,z:%f)", front.x, front.y, front.z);

	if (input_->PushKey(DIK_UP))
	{
		move = { front.x * -kCharacterSpeed, 0, front.z * -kCharacterSpeed };
	}
	if (input_->PushKey(DIK_DOWN))
	{
		move = { front.x * kCharacterSpeed, 0, front.z * kCharacterSpeed };
	}

	worldTransform_[PartId::Root].translation_.x += move.x;
	worldTransform_[PartId::Root].translation_.y += move.y;
	worldTransform_[PartId::Root].translation_.z += move.z;

	for (int i = 0; i < _countof(worldTransform_); i++)
	{
		worldTransform_[i].UpdateMatrix();
	}

#pragma endregion キャラクター移動処理

#pragma region 追従カメラ

	viewProjection_.target = worldTransform_[PartId::Root].translation_;
	viewProjection_.eye.x = worldTransform_[PartId::Root].translation_.x + front.x * 20;
	viewProjection_.eye.z = worldTransform_[PartId::Root].translation_.z + front.z * 20;

	viewProjection_.UpdateMatrix();

#pragma endregion 追従カメラ

	int balNum = 0;
	for (int i = 0; i < _countof(bal); i++)
	{
		bool j = bal[i].GetFlag();
		balNum += j == true;
		if (j == false)break;
	}

	if (input_->TriggerKey(DIK_SPACE) && balNum < _countof(bal))
	{
		bal[balNum].Create(worldTransform_[0].translation_, front);
	}

	for (int i = 0; i < _countof(bal); i++)
	{
		bal[i].Update();
	}

	for (int i = 0; i < 10; i++)
	{
		debugText_->SetPos(50, 70 + i * 20);
		debugText_->Printf("flag:%d", bal[i].GetFlag());
	}
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
	for (int i = 0; i < _countof(bal); i++)
	{
		bal[i].Draw(viewProjection_, textureHandle_);
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
