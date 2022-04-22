#pragma once
#include <DirectXMath.h>
#include "Model.h"

class Ballet
{
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:
	XMFLOAT3 pos = { 0,0,0 };
	XMFLOAT3 move = { 0,0,0 };
	int timer = 0;
	bool flag = false;
	Model* model_ = nullptr;
	WorldTransform balletTranslation_;

public:
	Ballet();
	~Ballet();
	void Create(XMFLOAT3 pos, XMFLOAT3 move);
	void Update(float speed = 0.5f, int timerMax = 100);
	void Draw(ViewProjection viewProjection_, uint32_t textureHandle_);
	bool GetFlag()const;
};