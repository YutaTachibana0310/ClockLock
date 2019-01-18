//=============================================================================
//
// スモールザコ1ヘッダ [SmallZako1.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _SMALLZAKO1_H_
#define _SMALLZAKO1_H_


// マクロ定義
#define SMALLZAKO1_TEXTURE_NAME	_T("data/TEXTURE/Enemy/sZako.png")			// スモールザコ1のテクスチャ
#define SMALLZAKO1_SHADOWTEX_NAME	_T("data/TEXTURE/Shadow/sZako1.png")	// スモールザコ1のテクスチャ(影)

#define SMALLZAKO1_TEXTURE_SIZE_X					(56)					// テクスチャサイズX
#define SMALLZAKO1_TEXTURE_SIZE_Y					(56)					// テクスチャサイズY

#define SMALLZAKO1_TEXTURE_NUM						(2)						//スモールザコのテクスチャの数

#define SMALLZAKO1_TEXTURE_DIVIDE_X					(2)						// アニメパターンのテクスチャ内分割数（X)
#define SMALLZAKO1_TEXTURE_DIVIDE_Y					(1)						// アニメパターンのテクスチャ内分割数（Y)
#define SMALLZAKO1_ANIM_NUM			(SMALLZAKO1_TEXTURE_DIVIDE_X*SMALLZAKO1_TEXTURE_DIVIDE_Y)	// アニメーションパターン数
#define SMALLZAKO1_TIME_ANIMATION					(2)						// アニメーションの切り替わるカウント

#define SMALLZAKO1_MAX								(56)					//スモールザコ1の最大数

#define SMALLZAKO1_INITPOS_X						(-100)					//スモールザコ1の初期X座標
#define SMALLZAKO1_INITPOS_Y						(-100)					//スモールザコ1の初期Y座標

//スモールザコ1のバウンディングボックスの定義
#define SMALLZAKO1_BB_TOP							(0.0f)
#define SMALLZAKO1_BB_LEFT							(0.0f)
#define SMALLZAKO1_BB_WIDTH							(25.0f)
#define SMALLZAKO1_BB_HIGHT							(25.0f)

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
}SMALLZAKO1;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSmallZako1(int num);							//スモールザコ1初期化処理
void UninitSmallZako1(void);								//スモールザコ1終了処理
void UpdateSmallZako1(void);								//スモールザコ1更新処理
void DrawSmallZako1(void);									//スモールザコ1描画処理
SMALLZAKO1 *GetSmallZako1Adr(int pno);						//スモールザコ1アドレス取得関数
void SetSmallZako1(D3DXVECTOR3 initPos);					//スモールザコ1セット関数

#endif
