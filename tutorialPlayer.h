//=============================================================================
//
// チュートリアルプレイヤーヘッダ [TutoPlayer.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _TUTOPLAYER_H_
#define _TUTOPLAYER_H_

#include "bulletFire.h"

// マクロ定義
#define TUTOPLAYER_TEXTURE_NAME	_T("data/TEXTURE/player/playerMainBullet.png")	// チュートリアルプレイヤーのテクスチャ

#define TUTOPLAYER_TEXTURE_SIZE_X					(70 / 2)					// テクスチャサイズX
#define TUTOPLAYER_TEXTURE_SIZE_Y					(80 / 2)					// テクスチャサイズY

#define TUTOPLAYER_TEXTURE_DIVIDE_X				(1)							// アニメパターンのテクスチャ内分割数（X)
#define TUTOPLAYER_TEXTURE_DIVIDE_Y				(1)							// アニメパターンのテクスチャ内分割数（Y)
#define TUTOPLAYER_ANIM_NUM			(TUTOPLAYER_TEXTURE_DIVIDE_X*TUTOPLAYER_TEXTURE_DIVIDE_Y)	// アニメーションパターン数
#define TUTOPLAYER_TIME_ANIMATION					(4)						// アニメーションの切り替わるカウント

#define TUTOPLAYER_MAX						(56)							//チュートリアルプレイヤーの最大数

#define TUTOPLAYER_INITPOS_X						(-100)					//チュートリアルプレイヤーの初期X座標
#define TUTOPLAYER_INITPOS_Y						(-100)					//チュートリアルプレイヤーの初期Y座標

#define TUTOPLAYER_BULLETFIRE_NUM					(4)
//チュートリアルプレイヤーのバウンディングボックスの定義
#define TUTOPLAYER_BB_TOP							(0.0f)
#define TUTOPLAYER_BB_LEFT						(0.0f)
#define TUTOPLAYER_BB_WIDTH						(10.0f)
#define TUTOPLAYER_BB_HIGHT						(35.0f)

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

	int						cntFrame;						//カウントフレーム
	bool					flgBomb;						//ボム使用フラグ

	int						shotInterbal;					//ショットインターバル
	int						numFireShot;					//サブショットの発射回数

	BULLETFIRE *fire[TUTOPLAYER_BULLETFIRE_NUM];			//バレット発射のエフェクト


}TUTOPLAYER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTutoPlayer(int num);							//チュートリアルプレイヤー初期化処理
void UninitTutoPlayer(void);								//チュートリアルプレイヤー終了処理
void UpdateTutoPlayer(void);								//チュートリアルプレイヤー更新処理
void DrawTutoPlayer(void);									//チュートリアルプレイヤー描画処理
TUTOPLAYER *GetTutoPlayerAdr(int pno);						//チュートリアルプレイヤーアドレス取得関数

#endif
