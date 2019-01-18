//=============================================================================
//
// エネミーパーティクルヘッダ [PlayerParticle.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _PLAYERPARTICLE_H_
#define _PLAYERPARTICLE_H_


// マクロ定義
#define PLAYERPARTICLE_TEXTURE_NAME	_T("data/TEXTURE/effect/playerParticle.png")	// エネミーパーティクルのテクスチャ

#define PLAYERPARTICLE_TEXTURE_SIZE_X					(8)					// テクスチャサイズX
#define PLAYERPARTICLE_TEXTURE_SIZE_Y					(8)					// テクスチャサイズY

#define PLAYERPARTICLE_MAX							(1024)					//エネミーパーティクルの最大数
#define PLAYERPARTICLE_EMITTER_MAX					(128)

#define PLAYERPARTICLE_INITPOS_X						(-100)					//エネミーパーティクルの初期X座標
#define PLAYERPARTICLE_INITPOS_Y						(-100)					//エネミーパーティクルの初期Y座標

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	bool					active;						//使用or未使用の判定

	D3DXVECTOR3				pos;						// ポリゴンの座標

	float					radius;						// ポリゴンの半径
	float					angle;						// ポリゴンの角度

	int						frameCnt;					//フレームカウント
	float					speed, initSpeed;
	D3DXVECTOR3				dir;
	float					scale, initScale;
	int						lifeFrame;
}PLAYERPARTICLE;

typedef struct {
	bool					active;

	D3DXVECTOR3				pos;

	int						frameCnt;
	int						duration;
	int						shape;
	int						emitNum;
	float					maxScale;
}EMITTER_PLAYERPARTICLE;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayerParticle(int num);							//エネミーパーティクル初期化処理
void UninitPlayerParticle(void);								//エネミーパーティクル終了処理
void UpdatePlayerParticle(void);								//エネミーパーティクル更新処理
void DrawPlayerParticle(void);								//エネミーパーティクル描画処理
PLAYERPARTICLE *GetPlayerParticleAdr(int pno);						//エネミーパーティクルアドレス取得関数
void SetPlayerParticle(D3DXVECTOR3 pos, int shape, float maxScale);
void SetEmitterPlayerParticle(D3DXVECTOR3 pos, int duration, int emitNum, int shape, float maxScale);

#endif
