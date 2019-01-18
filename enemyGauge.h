//=============================================================================
//
// エネミーゲージヘッダ [enemyGauge.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _ENEMYGAUGE_H_
#define _ENEMYGAUGE_H_


// マクロ定義
#define ENEMYGAUGE_TEXTURE_NAME	_T("data/TEXTURE/GUI/enemyGauge.png")			// エネミーゲージのテクスチャ
#define ENEMYGAUGEFRAME_TEXTURE_NAME	_T("data/TEXTURE/GUI/enemyGaugeFrame.png")	// エネミーゲージフレームのテクスチャ
#define ENEMYGAUGEBG_TEXTURE_NAME	_T("data/TEXTURE/GUI/enemyGaugeBG.png")		// エネミーゲージ背景のテクスチャ

#define ENEMYGAUGE_TEXTURE_SIZE_X					(100)					// テクスチャサイズX
#define ENEMYGAUGE_TEXTURE_SIZE_Y					(15)					// テクスチャサイズY

#define ENEMYGAUGE_TEXTURE_DIVIDE_X					(1)							// アニメパターンのテクスチャ内分割数（X)
#define ENEMYGAUGE_TEXTURE_DIVIDE_Y					(1)							// アニメパターンのテクスチャ内分割数（Y)
#define ENEMYGAUGE_ANIM_NUM			(ENEMYGAUGE_TEXTURE_DIVIDE_X*ENEMYGAUGE_TEXTURE_DIVIDE_Y)	// アニメーションパターン数
#define ENEMYGAUGE_TIME_ANIMATION					(4)							// アニメーションの切り替わるカウント

#define ENEMYGAUGE_MAX								(56)						//エネミーゲージの最大数

#define ENEMYGAUGE_INITPOS_X						(-100)						//エネミーゲージの初期X座標
#define ENEMYGAUGE_INITPOS_Y						(-100)						//エネミーゲージの初期Y座標

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	bool					active;							//使用or未使用の判定

	D3DXVECTOR3				pos;							// ポリゴンの座標
	D3DXVECTOR3				rot;							// ポリゴンの回転量
	int						countAnim;						// アニメーションカウント
	int						patternAnim;					// アニメーションパターンナンバー
	float					scale;							//ゲージのスケール

}ENEMYGAUGE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemyGauge(int num);
void UninitEnemyGauge(void);
void UpdateEnemyGauge(void);
void DrawEnemyGauge(void);
ENEMYGAUGE *GetEnemyGaugeAdr(int pno);
ENEMYGAUGE *GetNonActiveEnemyGauge(void);

#endif
