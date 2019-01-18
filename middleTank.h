//=============================================================================
//
// ミドルタンクヘッダ [MiddleTank.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _MIDDLETANK_H_
#define _MIDDLETANK_H_


// マクロ定義
#define MIDDLETANK_TEXTURE_NAME	_T("data/TEXTURE/Enemy/Tank.png")				// ミドルタンクのテクスチャ
#define MIDDLETANK_TURRETTEX_NAME _T("data/TEXTURE/Enemy/tankTurret.png")		//ミドルタンクのテクスチャ（砲塔）

#define MIDDLETANK_TEXTURE_SIZE_X					(56)						// テクスチャサイズX
#define MIDDLETANK_TEXTURE_SIZE_Y					(56)						// テクスチャサイズY

#define MIDDLETANK_TEXTURE_NUM						(2)							//テクスチャの数

#define MIDDLETANK_TEXTURE_DIVIDE_X					(2)							// アニメパターンのテクスチャ内分割数（X)
#define MIDDLETANK_TEXTURE_DIVIDE_Y					(2)							// アニメパターンのテクスチャ内分割数（Y)
#define MIDDLETANK_ANIM_NUM			(MIDDLETANK_TEXTURE_DIVIDE_X*MIDDLETANK_TEXTURE_DIVIDE_Y)	// アニメーションパターン数
#define MIDDLETANK_TIME_ANIMATION					(4)							// アニメーションの切り替わるカウント

#define MIDDLETANK_MAX								(56)						//ミドルタンクの最大数

#define MIDDLETANK_INITPOS_X						(-100)						//ミドルタンクの初期X座標
#define MIDDLETANK_INITPOS_Y						(-100)						//ミドルタンクの初期Y座標

#define MIDDLETANK_SETTER_MAX						(10)						//ミドルタンクセッターの最大数

//ミドルタンクのバウンディングボックスの定義
#define MIDDLETANK_BB_TOP							(0.0f)
#define MIDDLETANK_BB_LEFT							(0.0f)
#define MIDDLETANK_BB_WIDTH							(40.0f)
#define MIDDLETANK_BB_HIGHT							(40.0f)

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

	float					turretRot;						//砲塔の回転量
	int						countFrame;						//カウントフレーム
	D3DXVECTOR3				moveDir;						//進行方向
	int						state;							//行動状態
	int						hp;								//HP
	int						attackNum;						//攻撃回数
	int						shotWay;						//発射する弾のWAY数
}MIDDLETANK;

typedef struct {
	bool					active;							//使用or未使用
	D3DXVECTOR3				pos;							//座標
	D3DXVECTOR3				targetPos;						//目標座標
	int						cntFrame;						//フレームカウント
	int						cntSet;							//ミドルタンクをセットした回数
}MIDDLETANK_SETTER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMiddleTank(int num);								//ミドルタンク初期化処理
void UninitMiddleTank(void);									//ミドルタンク終了処理
void UpdateMiddleTank(void);									//ミドルタンク更新処理
void DrawMiddleTank(void);										//ミドルタンク描画処理
MIDDLETANK *GetMiddleTankAdr(int pno);							//ミドルタンクアドレス取得関数
void SetMiddleTank(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos);	//ミドルタンクのセット関数
void CreateMiddleTankSetter(D3DXVECTOR3 initPos, D3DXVECTOR3 target);	//ミドルタンクセッター設定関数
#endif
