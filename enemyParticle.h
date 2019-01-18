//=============================================================================
//
// エネミーパーティクルヘッダ [EnemyParticle.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _ENEMYPARTICLE_H_
#define _ENEMYPARTICLE_H_


// マクロ定義
#define ENEMYPARTICLE_TEXTURE_NAME	_T("data/TEXTURE/effect/enemyParticle.png")	// エネミーパーティクルのテクスチャ

#define ENEMYPARTICLE_TEXTURE_SIZE_X					(30)					// テクスチャサイズX
#define ENEMYPARTICLE_TEXTURE_SIZE_Y					(30)					// テクスチャサイズY

#define ENEMYPARTICLE_MAX							(1024)					//エネミーパーティクルの最大数
#define ENEMYPARTICLE_EMITTER_MAX					(128)

#define ENEMYPARTICLE_INITPOS_X						(-100)					//エネミーパーティクルの初期X座標
#define ENEMYPARTICLE_INITPOS_Y						(-100)					//エネミーパーティクルの初期Y座標

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
}ENEMYPARTICLE;

typedef struct {
	bool					active;

	D3DXVECTOR3				pos;

	int						frameCnt;
	int						duration;
	int						shape;
	int					emitNum;
}EMITTER_ENEMYPARTICLE;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemyParticle(int num);							//エネミーパーティクル初期化処理
void UninitEnemyParticle(void);								//エネミーパーティクル終了処理
void UpdateEnemyParticle(void);								//エネミーパーティクル更新処理
void DrawEnemyParticle(void);								//エネミーパーティクル描画処理
ENEMYPARTICLE *GetEnemyParticleAdr(int pno);						//エネミーパーティクルアドレス取得関数
void SetEnemyParticle(D3DXVECTOR3 pos, int shape);
void SetEmitterEnemyParticle(D3DXVECTOR3 pos, int duration, int emitNum, int shape);
#endif
