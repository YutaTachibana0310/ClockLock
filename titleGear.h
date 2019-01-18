//=============================================================================
//
// タイトルギアヘッダ [TitleGear.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _TITLEGEAR_H_
#define _TITLEGEAR_H_


// マクロ定義
#define TITLEGEAR_TEXTURE_NAME	_T("data/TEXTURE/bg/gear.png")	// タイトルギアのテクスチャ

#define TITLEGEAR_TEXTURE_SIZE_X				(100)					// テクスチャサイズX
#define TITLEGEAR_TEXTURE_SIZE_Y				(100)					// テクスチャサイズY
#define TITLEGEAR_MAX							(64)					//タイトルギアの最大数

#define TITLEGEAR_INITPOS_X						(-100)					//タイトルギアの初期X座標
#define TITLEGEAR_INITPOS_Y						(-100)					//タイトルギアの初期Y座標

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	bool					active;							// 使用or未使用の判定

	D3DXVECTOR3				pos;							// ポリゴンの座標
	D3DXVECTOR3				rot;							// ポリゴンの回転量
	float					scale;							// ポリゴンのスケール
	int						countAnim;						// アニメーションカウント
	int						rotDir;							// 歯車の回転方向
}TITLEGEAR;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTitleGear(int num);							//タイトルギア初期化処理
void UninitTitleGear(void);								//タイトルギア終了処理
void UpdateTitleGear(void);								//タイトルギア更新処理
void DrawTitleGear(void);								//タイトルギア描画処理
TITLEGEAR *GetTitleGearAdr(int pno);					//タイトルギアアドレス取得関数

#endif
