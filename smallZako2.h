//=============================================================================
//
// スモールザコ2ヘッダ [SmallZako2.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _SMALLZAKO2_H_
#define _SMALLZAKO2_H_


// マクロ定義
#define SMALLZAKO2_TEXTURE_NAME	_T("data/TEXTURE/Enemy/sZako.png")			// スモールザコ2のテクスチャ
#define SMALLZAKO2_SHADOWTEX_NAME	_T("data/TEXTURE/Shadow/sZako1.png")	// スモールザコ2のテクスチャ(影)

#define SMALLZAKO2_TEXTURE_SIZE_X					(56)					// テクスチャサイズX
#define SMALLZAKO2_TEXTURE_SIZE_Y					(56)					// テクスチャサイズY

#define SMALLZAKO2_TEXTURE_NUM						(2)						//スモールザコのテクスチャの数

#define SMALLZAKO2_TEXTURE_DIVIDE_X					(2)						// アニメパターンのテクスチャ内分割数（X)
#define SMALLZAKO2_TEXTURE_DIVIDE_Y					(1)						// アニメパターンのテクスチャ内分割数（Y)
#define SMALLZAKO2_ANIM_NUM			(SMALLZAKO2_TEXTURE_DIVIDE_X*SMALLZAKO2_TEXTURE_DIVIDE_Y)	// アニメーションパターン数
#define SMALLZAKO2_TIME_ANIMATION					(2)						// アニメーションの切り替わるカウント

#define SMALLZAKO2_MAX								(56)					//スモールザコ2の最大数

#define SMALLZAKO2_INITPOS_X						(-100)					//スモールザコ2の初期X座標
#define SMALLZAKO2_INITPOS_Y						(-100)					//スモールザコ2の初期Y座標

//スモールザコ2のバウンディングボックスの定義
#define SMALLZAKO2_BB_TOP							(0.0f)
#define SMALLZAKO2_BB_LEFT							(0.0f)
#define SMALLZAKO2_BB_WIDTH							(25.0f)
#define SMALLZAKO2_BB_HIGHT							(25.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	bool					active;							//使用or未使用の判定

	D3DXVECTOR3				pos;							// ポリゴンの座標
	D3DXVECTOR3				rot;							// ポリゴンの回転量
	int						countAnim;						// アニメーションカウント
	int						patternAnim;					// アニメーションパターンナンバー

	int						hp;								//HP
	D3DXVECTOR3				moveDir;						//進行方向
	float					speed;							//移動スピード
	int						state;							//行動状態
	int						cntFrame;						//カウントフレーム
	int						cntAttack;						//攻撃回数
}SMALLZAKO2;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSmallZako2(int num);							//スモールザコ2初期化処理
void UninitSmallZako2(void);								//スモールザコ2終了処理
void UpdateSmallZako2(void);								//スモールザコ2更新処理
void DrawSmallZako2(void);									//スモールザコ2描画処理
SMALLZAKO2 *GetSmallZako2Adr(int pno);						//スモールザコ2アドレス取得関数
void SetSmallZako2(D3DXVECTOR3 initPos);					//スモールザコ2セット関数

#endif
