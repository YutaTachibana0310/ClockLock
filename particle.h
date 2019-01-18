//=============================================================================
//
// パーティクルヘッダー [particle.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "effect.h"

// マクロ定義
#define PARTICLE_MAX					(56)				//パーティクルの最大数

/*****************************************************************************
//構造体定義
*****************************************************************************/
enum PARTICLETYPE
{
	PIECE,
	FIRE,
	EXPLOSION,
	BOSSFIRE,
	KIND_MAX
};

typedef struct 
{	
	bool				active;				//ゲーム内でアクティブかどうか

	D3DXVECTOR3			pos;				//座標

	int					cntFrame;			//セットされてからの経過フレーム

	EFFECT				*particle;			//パーティクルを構成するエフェクトへのポインタ
}PARTICLE;

/*****************************************************************************
// プロトタイプ宣言
*****************************************************************************/
HRESULT InitParticle(int num);					//パーティクル初期化処理
void UninitParticle(void);						//パーティクル終了処理
void UpdateParticle(void);						//パーティクル更新処理
void DrawParticle(void);						//パーティクル描画処理
PARTICLE *GetParticleAdr(int type, int pno);	//パーティクルアドレス取得関数
void SetParticle(D3DXVECTOR3 pos, int type);	//爆発パーティクルのセット関数

#endif
