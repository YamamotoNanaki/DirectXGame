#include "Ballet.h"

Ballet::Ballet()
{
	model_ = Model::Create();
	balletTranslation_.scale_ = { 0.2,0.2,0.2 };
	balletTranslation_.Initialize();
}

Ballet::~Ballet()
{
	delete model_;
}

void Ballet::Create(XMFLOAT3 pos, XMFLOAT3 move)
{
	this->pos = pos;
	this->move = move;
	flag = true;
}

void Ballet::Update(float speed, int timerMax)
{
	if (flag == true)
	{
		pos.x -= move.x * speed;
		pos.y += move.y * speed;
		pos.z -= move.z * speed;

		balletTranslation_.translation_ = pos;
		balletTranslation_.UpdateMatrix();

		timer++;
		if (timer >= timerMax)
		{
			flag = false;
			timer = 0;
		}
	}
}

void Ballet::Draw(ViewProjection viewProjection_, uint32_t textureHandle_)
{
	if (flag == true)model_->Draw(balletTranslation_, viewProjection_, textureHandle_);
}

bool Ballet::GetFlag() const
{
	return flag;
}
