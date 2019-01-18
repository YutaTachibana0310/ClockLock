//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "particle.h"
#include "effectExplosion.h"
#include "effectPiece.h"
#include "effectFire.h"
#include "input.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define PARTICLE_FIRE_EMITTFRAME			(12)			//ファイアパーティクルの放出時間
#define PARTICLE_PIECE_EMITTFRAME			(6)				//ピースパーティクルの放出時間
#define PARTICLE_EXPLOSION_EMITTFRAME		(12)			//爆発パーティクルの放出時間
#define PARTICLE_BOSSFIRE_EMITTFRAME		(6)				//ボス爆発パーティクルの放出時間

#define PARTICLE_FIRE_EMITTNUM				(3)				//ファイアパーティクルの放出数
#define PARTICLE_PIECE_EMITTNUM				(6)				//ピースパーティクルの放出数
#define PARTICLE_EXPLOSION_EMITTNUM			(2)				//爆発パーティクルの放出数
#define PARTICLE_BOSSFIRE_EMITTNUM			(12)			//ボス爆発パーティクルの放出数

#define PARTICLE_BOSSFIRE_INITSPEED			(4.0f)			//ボスファイアの初期スピード
#define PARTICLE_BOSSFIRE_SPEEDOFFSET		(3.0f)			//ボスファイアのスピード差分

#define PARTICLE_PIECE_ANGLEMIN				(30.0f)			//ピースパーティクルの角度最小値
#define PARTICLE_PIECE_ANGLEMAX				(150.0f)		//ピースパーティクルの角度最大値

#define PARTICLE_FIRE_ANGLEMIN				(0.0f)			//ファイアパーティクルの角度最小値
#define PARTICLE_FIRE_ANGLEMAX				(360.0f)		//ファイアパーティクルの角度最大値

#define PARTICLE_EXPLOSION_ANGLEMIN			(0.0f)			//エクスプロージョンパーティクルの角度最小値
#define PARTICLE_EXPLOSION_ANGLEMAX			(360.0f)		//エクスプロージョンパーティクルの角度最大値

//#define PARTICLE_FIRE_PARTICLENUM			(10)			//ファイアパーティクルのパーティクル数
//#define PARTICEL_PIECE_PARTICLENUM			(40)			//ピースパーティクルのパーティクル数
//#define PARTICLE_EXPLOSION_PARTICLENUM		(30)			//爆発パーティクルのパーティクル数

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/

/*****************************************************************************
構造体定義
****************************************************************************/

/*****************************************************************************
グローバル変数
****************************************************************************/
static PARTICLE particle[KIND_MAX][PARTICLE_MAX];			//パーティクル構造体配列

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitParticle(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PARTICLE *ptr = GetParticleAdr(0, 0);
	int i;

	for (i = 0; i < KIND_MAX*PARTICLE_MAX; i++, ptr++)
	{
		ptr->pos = VECTOR3_ZERO;
		ptr->cntFrame = 0;
		ptr->active = false;
	}
	
	InitEffect(num);

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitParticle(void)
{
	UninitEffect();
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateParticle(void)
{
	PARTICLE *ptr = GetParticleAdr(0, 0);
	int i, j, k;
	D3DXVECTOR3 dir = VECTOR3_ZERO;
	float tmp;

	/*//確認用デバッグ機能
	if (GetKeyboardTrigger(DIK_SPACE))
	{
		//SetParticle(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), EXPLOSION);
		//SetParticle(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), PIECE);
		//SetParticle(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), FIRE);
		SetParticle(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), BOSSFIRE);
	}
	*/

	for (i = 0; i < KIND_MAX; i++)
	{
		for (j = 0; j < PARTICLE_MAX; j++, ptr++)
		{
			//非アクティブのパーティクルなら処理を行わない
			if (!ptr->active)
			{
				continue;
			}

			//パーティクルの種類によって処理を分岐
			switch (i)
			{
			case FIRE:
				for (k = 0; k < PARTICLE_FIRE_EMITTNUM; k++)
				{
					tmp = RandomRange(PARTICLE_FIRE_ANGLEMIN, PARTICLE_FIRE_ANGLEMAX);
					dir.x = cosf(RADIAN(tmp));
					dir.y = sinf(RADIAN(tmp));
					SetEffectFire(ptr->pos, dir);

					ptr->cntFrame++;
					if (ptr->cntFrame >= PARTICLE_FIRE_EMITTFRAME)
					{
						ptr->active = false;
					}
				}
				break;

			case PIECE:
				for (k = 0; k < PARTICLE_PIECE_EMITTNUM; k++)
				{
					tmp = RandomRange(PARTICLE_PIECE_ANGLEMIN, PARTICLE_PIECE_ANGLEMAX);
					dir.x = cosf(RADIAN(tmp));
					dir.y = sinf(RADIAN(tmp));
					SetEffectPiece(ptr->pos, dir);
				}

				ptr->cntFrame++;
				if (ptr->cntFrame >= PARTICLE_PIECE_EMITTFRAME)
				{
					ptr->active = false;
				}
				break;

			case EXPLOSION:
				for (k = 0; k < PARTICLE_EXPLOSION_EMITTNUM; k++)
				{
					tmp = RandomRange(PARTICLE_EXPLOSION_ANGLEMIN, PARTICLE_EXPLOSION_ANGLEMAX);
					dir.x = cosf(RADIAN(tmp));
					dir.y = sinf(RADIAN(tmp));
					SetEffectExplosion(ptr->pos, dir);
				}

				ptr->cntFrame++;
				if (ptr->cntFrame >= PARTICLE_EXPLOSION_EMITTFRAME)
				{
					ptr->active = false;
				}
				break;

			case BOSSFIRE:
				tmp = RandomRange(PARTICLE_FIRE_ANGLEMIN, PARTICLE_FIRE_ANGLEMAX);
				dir.x = cosf(RADIAN(tmp));
				dir.y = sinf(RADIAN(tmp));
				for (k = 0; k < PARTICLE_BOSSFIRE_EMITTNUM; k++)
				{
					SetBossFire(ptr->pos, dir, PARTICLE_BOSSFIRE_INITSPEED + k * PARTICLE_BOSSFIRE_SPEEDOFFSET);
				}

				ptr->cntFrame++;
				if (ptr->cntFrame >= PARTICLE_BOSSFIRE_EMITTFRAME)
				{
					ptr->active = false;
				}
				break;
			}
		}
	}

	UpdateEffect();
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	DrawEffect();
}

/******************************************************************************
//関数名	：PARTICLE *GetParticleAdr(int type, int pno)
//引数1		：int type	：取得したいパーティクルの種類
//引数2		：int pno	：取得したいパーティクルのインデックス
//戻り値	：void
//説明		：中型爆発のアドレス取得関数
******************************************************************************/
PARTICLE *GetParticleAdr(int type, int pno)
{
	return &particle[type][pno];
}

/******************************************************************************
//関数名	：void FireParticle(D3DXVECTOR3 pos)
//引数1		：D3DXVECTOR3 pos	：セットしたい座標
//引数2		：int type			：セットしたいパーティクルのタイプ
//戻り値	：void
//説明		：パーティクルをゲーム内にセットする関数
******************************************************************************/
void SetParticle(D3DXVECTOR3 pos, int type)
{
	PARTICLE *ptr = GetParticleAdr(type, 0);
	int i;

	for (i = 0; i < PARTICLE_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			ptr->pos = pos;
			ptr->cntFrame = 0;
			ptr->active = true;
			return;
		}
	}
}