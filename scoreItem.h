//=============================================================================
//
// スコアアイテムヘッダ [ScoreItem.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _SCOREITEM_H_
#define _SCOREITEM_H_


// マクロ定義
#define SCOREITEM_TEXTURE_NAME			_T("data/TEXTURE/player/item.png")			// スコアアイテムのテクスチャ
#define SCOREITEMLUMINE_TEXTURE_NAME	_T("data/TEXTURE/player/itemLumine.png")	// スコアアイテム光のテクスチャ

#define SCOREITEM_TEXTURE_NUM						(2)							//スコアアイテムのテクスチャの数

#define SCOREITEM_TEXTURE_SIZE_X					(120 / 2)					// テクスチャサイズX
#define SCOREITEM_TEXTURE_SIZE_Y					(120 / 2)					// テクスチャサイズY

#define SCOREITEM_TEXTURE_DIVIDE_X					(8)							// アニメパターンのテクスチャ内分割数（X)
#define SCOREITEM_TEXTURE_DIVIDE_Y					(2)							// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_SCOREITEM_NUM							(7)							// アニメーションパターン数
#define TIMESCOREITEM_ANIMATION						(2)							// アニメーションの切り替わるカウント

#define SCOREITEM_MAX								(4096)						//スコアアイテムの最大数

#define SCOREITEM_INITPOS_X							(-100)						//スコアアイテムの初期X座標
#define SCOREITEM_INITPOS_Y							(-100)						//スコアアイテムの初期Y座標

#define SCOREITEM_ADDVALUE							(500)						//スコアアイテム取得時のスコア加算値

//スコアアイテムのバウンディングボックスの定義
#define SCOREITEM_BB_TOP							(0.0f)
#define SCOREITEM_BB_LEFT							(0.0f)
#define SCOREITEM_BB_WIDTH							(15.0f)
#define SCOREITEM_BB_HIGHT							(15.0f)

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
	float					baseRadius;						//ポリゴンの半径（変化させない）
	float					angle;							// ポリゴンの角度

	D3DXVECTOR3				velocity;						//アイテムの進行方向
	int						cntFrame;						//フレームカウント
	int						state;							//アイテムの状態

	float					scale;

}SCOREITEM;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitScoreItem(int num);								//スコアアイテム初期化処理
void UninitScoreItem(void);									//スコアアイテム終了関数
void UpdateScoreItem(void);									//スコアアイテム更新処理
void DrawScoreItem(void);									//スコアアイテム描画処理
SCOREITEM *GetScoreItemAdr(int pno);						//スコアアイテムアドレス取得関数
void SetScoreItem(D3DXVECTOR3 pos, float range, int num);	//スコアアイテムセット関数
int GetScoreItemCount(void);								//スコアアイテム取得数のゲット関数
void SetScoreItemCount(int value);							//スコアアイテム取得数のセット関数
float GetScoreItemScale(void);							//スケールステート取得関数

#ifdef _DEBUG
int GetActiveItemCount(void);								//アクティブなスコアアイテム数取得関数
void SetActiveItemCount(int value);							//スコアアイテム数操作関数
#endif

#endif
