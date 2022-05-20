#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene()
{
	delete model;
	delete dCamera;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	texH = TextureManager::Load("mario.jpg");
	model = Model::Create();
	world.Initialize();
	vp.Initialize();
	dCamera = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&dCamera->GetViewProjection());
	PrimitiveDrawer::GetInstance()->SetViewProjection(&dCamera->GetViewProjection());
}

void GameScene::Update()
{
	dCamera->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	for (int i = -10; i < 11; i++)
	{
		PrimitiveDrawer::GetInstance()->DrawLine3d({ -20,(float)i * 2,0 }, { 20,(float)i * 2,0 }, { 1.0,0.1,0.1,1.0 });
		PrimitiveDrawer::GetInstance()->DrawLine3d({ (float)i * 2,-20,0 }, { (float)i * 2,20,0 }, { 0.1,1.0,0.1,1.0 });
	}

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
	model->Draw(world, dCamera->GetViewProjection(), texH);

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
