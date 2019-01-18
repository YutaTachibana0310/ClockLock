//=============================================================================
//
// ボスゲージヘッダ [BossGauge.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _BOSSGAUGE_H_
#define _BOSSGAUGE_H_


// マクロ定義
#define BOSSGAUGE_TEXTURE_NAME	_T("data/TEXTURE/GUI/bossGauge.png")	// ボスゲージのテクスチャ

#define BOSSGAUGE_TEXTURE_SIZE_X					(390)					// テクスチャサイズX
#define BOSSGAUGE_TEXTURE_SIZE_Y					(30)					// テクスチャサイズY

#define BOSSGAUGE_TEXTURE_DIVIDE_X					(1)						// アニメパターンのテクスチャ内分割数（X)
#define BOSSGAUGE_TEXTURE_DIVIDE_Y					(2)						// アニメパターンのテクスチャ内分割数（Y)

#define BOSSGAUGE_INITPOS_X							(SCREEN_CENTER_X)		//ボスゲージの初期X座標
#define BOSSGAUGE_INITPOS_Y							(25)					//ボスゲージの初期Y座標

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	bool					active;						//使用or未使用の判定

	D3DXVECTOR3				pos;						// ポリゴンの座標
	D3DXVECTOR3				rot;						// ポリゴンの回転量

	float					radius;						// ポリゴンの半径
	float					angle;						// ポリゴンの角度

	int						state;						//状態
	float					scale;						//スケール
	float					percent;					//残り割合

}BOSSGAUGE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBossGauge(int num);							//ボスゲージ初期化処理
void UninitBossGauge(void);								//ボスゲージ終了処理
void UpdateBossGauge(void);								//ボスゲージ更新処理
void DrawBossGauge(void);								//ボスゲージ描画処理
BOSSGAUGE *GetBossGaugeAdr(void);						//ボスゲージアドレス取得関数
BOSSGAUGE *SetBossGauge(void);							//ボスゲージセット関数

#endif
