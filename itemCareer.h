//=============================================================================
//
// アイテムキャリアヘッダ [ItemCareer.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _ITEMCAREER_H_
#define _ITEMCAREER_H_

// マクロ定義
#define ITEMCAREER_TEXTURE_NAME	_T("data/TEXTURE/Enemy/ItemCareer.png")		// アイテムキャリアのテクスチャ
#define ITEMCAREER_SHADOWTEX_NAME _T("data/TEXTURE/Shadow/ItemCareer.png")	//影のテクスチャ

#define ITEMCAREER_TEXTURE_SIZE_X					(84)					// テクスチャサイズX
#define ITEMCAREER_TEXTURE_SIZE_Y					(84)					// テクスチャサイズY

#define ITEMCAREER_TEXTURE_DIVIDE_X					(2)						// アニメパターンのテクスチャ内分割数（X)
#define ITEMCAREER_TEXTURE_DIVIDE_Y					(1)						// アニメパターンのテクスチャ内分割数（Y)
#define ITEMCAREER_ANIM_NUM			(ITEMCAREER_TEXTURE_DIVIDE_X*ITEMCAREER_TEXTURE_DIVIDE_Y)	// アニメーションパターン数
#define ITEMCAREER_TIME_ANIMATION					(2)						// アニメーションの切り替わるカウント

#define ITEMCAREER_MAX								(4)						//アイテムキャリアの最大数

#define ITEMCAREER_INITPOS_X						(-100)					//アイテムキャリアの初期X座標
#define ITEMCAREER_INITPOS_Y						(-100)					//アイテムキャリアの初期Y座標

//アイテムキャリアのバウンディングボックスの定義
#define ITEMCAREER_BB_TOP							(0.0f)
#define ITEMCAREER_BB_LEFT						(0.0f)
#define ITEMCAREER_BB_WIDTH						(40.0f)
#define ITEMCAREER_BB_HIGHT						(70.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	bool					active;							//使用or未使用の判定

	D3DXVECTOR3				pos;							// ポリゴンの座標
	D3DXVECTOR3				rot;							// ポリゴンの回転量
	int						countAnim;						// アニメーションカウント
	int						patternAnim;					// アニメーションパターンナンバー
	int						state;							// 状態

	D3DXVECTOR3				targetPos;						//目標座標
	int						cntFrame;						//カウントフレーム

	int						hp;
}ITEMCAREER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitItemCareer(int num);							//アイテムキャリア初期化処理
void UninitItemCareer(void);								//アイテムキャリア終了処理
void UpdateItemCareer(void);								//アイテムキャリア更新処理
void DrawItemCareer(void);									//アイテムキャリア描画処理
ITEMCAREER *GetItemCareerAdr(int pno);						//アイテムキャリアアドレス取得関数
void SetItemCareer(D3DXVECTOR3 pos);						//アイテムキャリアセット関数
#endif
