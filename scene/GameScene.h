﻿#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	uint32_t textureHandle_ = 0;
	Sprite* sprite_ = nullptr;
	Model* model_ = nullptr;
	WorldTransform worldTransform_[9];
	ViewProjection viewProjection_;
	uint32_t soundDataHandle_ = 0;
	uint32_t voiceHandle_ = 0;
	int32_t value_ = 0;
	DirectX::XMFLOAT3 angle[3];
	float viewAngle = 0;
	int flag = 0;


	enum PartId
	{
		Root,
		Spine,
		Chest,
		Head,
		ArmL,
		ArmR,
		Hip,
		LegL,
		LegR,
	};

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
