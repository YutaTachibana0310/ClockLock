//=============================================================================
//
// BGオブジェヘッダ [BgObj.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _BGOBJ_H_
#define _BGOBJ_H_

// マクロ定義
#define BGOBJ_TEXTURE_NAME	_T("data/TEXTURE/bg/stage1_bg_obj.png")	// BGオブジェのテクスチャ

#define BGOBJ_TEXTURE_SIZE_X					(BG_TEXTURESIZE_X)		// テクスチャサイズX
#define BGOBJ_TEXTURE_SIZE_Y					(BG_TEXTURESIZE_Y)		// テクスチャサイズY

#define BGOBJ_TEXTURE_DIVIDE_X					(4)						// アニメパターンのテクスチャ内分割数（X)
#define BGOBJ_TEXTURE_DIVIDE_Y					(3)						// アニメパターンのテクスチャ内分割数（Y)

#define BGOBJ_MAX								(12)					//BGオブジェの最大数

#define BGOBJ_INITPOS_X							(BG_LEFT)				//BGオブジェの初期X座標
#define BGOBJ_INITPOS_Y							(0.0f)					//BGオブジェの初期Y座標

#define BGOBJ_SPEED				(SCREEN_HEIGHT * -BG_STAGE1_SPEED3)		//スクロールスピード
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	D3DXVECTOR3				pos;							// ポリゴンの座標
	int						patternAnim;					// アニメーションパターンナンバー
}BGOBJ;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBgObj(int num);							//BGオブジェ初期化処理
void UninitBgObj(void);								//BGオブジェ終了処理
void UpdateBgObj(void);								//BGオブジェ更新処理
void DrawBgObj(void);								//BGオブジェ描画処理
BGOBJ *GetBgObjAdr(int pno);						//BGオブジェアドレス取得関数

#endif
