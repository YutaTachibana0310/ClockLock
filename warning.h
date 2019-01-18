//=============================================================================
//
// ワーニングヘッダ [Warning.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _WARNING_H_
#define _WARNING_H_


// マクロ定義
#define WARNING_TEXTURE_NAME	_T("data/TEXTURE/GUI/warning.png")		// ワーニングのテクスチャ
#define WARNING_ROLLTEX_NAME	_T("data/TEXTURE/GUI/warningBelt.png")	// ワーニングの帯のテクスチャ
#define WARNING_BG_NAME			_T("data/TEXTURE/GUI/warningBG.png")	// ワーニングの背景テクスチャ

#define WARNING_TEXTURE_SIZE_X					(320)					// テクスチャサイズX
#define WARNING_TEXTURE_SIZE_Y					(65)					// テクスチャサイズY
#define WARNING_TEXTURE_DIVIDE_Y				(2)						// テクスチャのY分割数

#define WARNING_ROLL_SIZE_X						(320)					//ワーニング帯のサイズX
#define WARNING_ROLL_SIZE_Y						(15)					//ワーニング帯のサイズY

#define WARNING_INITPOS_X						(SCREEN_CENTER_X)		//ワーニングの初期X座標
#define WARNING_INITPOS_Y						(300)					//ワーニングの初期Y座標

#define WARNING_BG_SIZE_X						(390)					//ワーニング背景のサイズX
#define WARNING_BG_SIZE_Y						(480)					//ワーニング背景のサイズY
#define WARNING_BG_INITPOS_X					(SCREEN_CENTER_X)		//ワーニング背景の初期座標X
#define WARNING_BG_INITPOS_Y					(SCREEN_CENTER_Y)		//ワーニング背景の初期座標Y

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	bool					active;							//使用or未使用の判定
	int						countFrame;						// フレームカウント
	int						state;							// 状態
	float					radius;							// ポリゴンの半径
	float					angle;							// ポリゴンの角度

	float					radiusRoll;						// 帯ポリゴンの半径
	float					angleRoll;						// 帯ポリゴンの角度

	float					radiusBG;						//背景ポリゴンの半径
	float					angleBG;						//背景ポリゴンの角度

	float					offsetUV;						// 帯のテクスチャオフセット値
	float					offsetPos;						// 帯の頂点オフセット値

	int						countDiffuse;
	bool					flgDiffuse;
}WARNING;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitWarning(int num);							//ワーニング初期化処理
void UninitWarning(void);								//ワーニング終了処理
void UpdateWarning(void);								//ワーニング更新処理
void DrawWarning(void);									//ワーニング描画処理
WARNING *GetWarningAdr(void);							//ワーニングアドレス取得関数
void SetWarning(void);									//ワーニングセット関数

#endif
