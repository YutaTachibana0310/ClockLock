//=============================================================================
//
// スモールタンクヘッダ [SmallTank.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _SMALLTANK_H_
#define _SMALLTANK_H_


// マクロ定義
#define SMALLTANK_TEXTURE_NAME	_T("data/TEXTURE/Enemy/sTank.png")				// スモールタンクのテクスチャ
#define SMALLTANK_TURRETTEX_NAME _T("data/TEXTURE/Enemy/sTankTurret.png")		//スモールタンクのテクスチャ（砲塔）

#define SMALLTANK_TEXTURE_SIZE_X					(56)						// テクスチャサイズX
#define SMALLTANK_TEXTURE_SIZE_Y					(56)						// テクスチャサイズY

#define SMALLTANK_TEXTURE_NUM						(2)							//テクスチャの数

#define SMALLTANK_TEXTURE_DIVIDE_X					(2)							// アニメパターンのテクスチャ内分割数（X)
#define SMALLTANK_TEXTURE_DIVIDE_Y					(2)							// アニメパターンのテクスチャ内分割数（Y)
#define SMALLTANK_ANIM_NUM			(SMALLTANK_TEXTURE_DIVIDE_X*SMALLTANK_TEXTURE_DIVIDE_Y)	// アニメーションパターン数
#define SMALLTANK_TIME_ANIMATION					(4)							// アニメーションの切り替わるカウント

#define SMALLTANK_MAX								(56)						//スモールタンクの最大数

#define SMALLTANK_INITPOS_X							(-100)						//スモールタンクの初期X座標
#define SMALLTANK_INITPOS_Y							(-100)						//スモールタンクの初期Y座標

#define SMALLTANK_SETTER_MAX						(10)						//セッター最大数
//スモールタンクのバウンディングボックスの定義
#define SMALLTANK_BB_TOP							(0.0f)
#define SMALLTANK_BB_LEFT							(0.0f)
#define SMALLTANK_BB_WIDTH							(40.0f)
#define SMALLTANK_BB_HIGHT							(40.0f)

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
}SMALLTANK;

typedef struct {
	bool					active;							//使用or未使用
	D3DXVECTOR3				pos;							//座標
	D3DXVECTOR3				targetPos;						//目標座標
	int						cntFrame;						//フレームカウント
	int						cntSet;							//ミドルタンクをセットした回数
}SMALLTANK_SETTER;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSmallTank(int num);								//スモールタンク初期化処理
void UninitSmallTank(void);									//スモールタンク終了処理
void UpdateSmallTank(void);									//スモールタンク更新処理
void DrawSmallTank(void);										//スモールタンク描画処理
SMALLTANK *GetSmallTankAdr(int pno);							//スモールタンクアドレス取得関数
void SetSmallTank(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos);	//スモールタンクのセット関数
void CreateSmallTankSetter(D3DXVECTOR3 initPos, D3DXVECTOR3 target); //スモールタンクのセッター設定関数
#endif
