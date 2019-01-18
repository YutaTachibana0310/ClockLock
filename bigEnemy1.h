//=============================================================================
//
// ビッグエネミー1ヘッダ [BigEnemy1.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _BIGENEMY1_H_
#define _BIGENEMY1_H_


// マクロ定義
#define BIGENEMY1_TEXTURE_NAME	_T("data/TEXTURE/Enemy/bigWing1.png")	// ビッグエネミー1のテクスチャ
#define BIGENEMY1_SHADOWTEX_NAME	_T("data/TEXTURE/Shadow/bigWing1.png")	// ビッグエネミー1のテクスチャ(影用)

#define BIGENEMY1_TEXTURE_SIZE_X					(560 / 2)					// テクスチャサイズX
#define BIGENEMY1_TEXTURE_SIZE_Y					(210 / 2)					// テクスチャサイズY

#define BIGENEMY1_TEXTURE_NUM						(2)							//テクスチャの数

#define BIGENEMY1_TEXTURE_DIVIDE_X				(1)								// アニメパターンのテクスチャ内分割数（X)
#define BIGENEMY1_TEXTURE_DIVIDE_Y				(2)								// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_BIGENEMY1_NUM			(BIGENEMY1_TEXTURE_DIVIDE_X*BIGENEMY1_TEXTURE_DIVIDE_Y)	// アニメーションパターン数
#define TIMEBIGENEMY1_ANIMATION					(2)								// アニメーションの切り替わるカウント

#define BIGENEMY1_MAX							(5)								//ビッグエネミー1の最大数

#define BIGENEMY1_INITPOS_X						(-100)							//ビッグエネミー1の初期X座標
#define BIGENEMY1_INITPOS_Y						(-100)							//ビッグエネミー1の初期Y座標

//ビッグエネミー1のバウンディングボックスの定義
#define BIGENEMY1_BB_TOP						(0.0f)
#define BIGENEMY1_BB_LEFT						(0.0f)
#define BIGENEMY1_BB_WIDTH						(250.0f)
#define BIGENEMY1_BB_HIGHT						(50.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	bool					active;							//使用or未使用の判定

	D3DXVECTOR3				pos;							// ポリゴンの座標
	D3DXVECTOR3				rot;							// ポリゴンの回転量
	int						countAnim;						// アニメーションカウント
	int						patternAnim;					// アニメーションパターンナンバー

	float					radius;							// ポリゴンの半径
	float					angle;							// ポリゴンの角度

	D3DXVECTOR3				targetPos;						//移動の目標座標

	int						state;							//行動状態
	int						cntFrame;						//カウントフレーム

	int						hp;								//HP

	ENEMYGAUGE				*gauge;							//HPゲージへのポインタ

}BIGENEMY1;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBigEnemy1(int num);										//ビッグエネミー1初期化処理
void UninitBigEnemy1(void);											//ビッグエネミー1終了処理
void UpdateBigEnemy1(void);											//ビッグエネミー1更新処理
void DrawBigEnemy1(void);											//ビッグエネミー1描画処理
BIGENEMY1 *GetBigEnemy1Adr(int pno);								//ビッグエネミー1アドレス取得関数
void SetBigEnemy(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos);		//ビッグエネミーをゲーム内に出現させる関数

#endif
