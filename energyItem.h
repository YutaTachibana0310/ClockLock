//=============================================================================
//
// エナジーアイテムヘッダ [EnergyItem.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _ENERGYITEM_H_
#define _ENERGYITEM_H_


// マクロ定義
#define ENERGYITEM_TEXTURE_NAME	_T("data/TEXTURE/player/energyItem.png")	// エナジーアイテムのテクスチャ
#define ENERGYITEM_SHADOWTEX_NAME _T("data/TEXTURE/player/energyItemShadow.png") //影のテクスチャ

#define ENERGYITEM_TEXTURE_SIZE_X					(64)					// テクスチャサイズX
#define ENERGYITEM_TEXTURE_SIZE_Y					(64)					// テクスチャサイズY

#define ENERGYITEM_TEXTURE_DIVIDE_X					(2)						// アニメパターンのテクスチャ内分割数（X)
#define ENERGYITEM_TEXTURE_DIVIDE_Y					(1)						// アニメパターンのテクスチャ内分割数（Y)
#define ENERGYITEM_ANIM_NUM			(ENERGYITEM_TEXTURE_DIVIDE_X*ENERGYITEM_TEXTURE_DIVIDE_Y)	// アニメーションパターン数
#define ENERGYITEM_TIME_ANIMATION					(2)						// アニメーションの切り替わるカウント

#define ENERGYITEM_MAX								(8)					//エナジーアイテムの最大数

#define ENERGYITEM_INITPOS_X						(-100)					//エナジーアイテムの初期X座標
#define ENERGYITEM_INITPOS_Y						(-100)					//エナジーアイテムの初期Y座標

#define ENERGYITEM_ADDVALUE							(PLAYER_TIMEEFFECT_MAX/2)	//取得時に加算されるエナジー

//エナジーアイテムのバウンディングボックスの定義
#define ENERGYITEM_BB_TOP							(0.0f)
#define ENERGYITEM_BB_LEFT							(0.0f)
#define ENERGYITEM_BB_WIDTH							(10.0f)
#define ENERGYITEM_BB_HIGHT							(35.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	bool					active;							//使用or未使用の判定

	D3DXVECTOR3				pos;							// ポリゴンの座標
	D3DXVECTOR3				rot;							// ポリゴンの回転量
	int						countAnim;						// アニメーションカウント
	int						patternAnim;					// アニメーションパターンナンバー

	D3DXVECTOR3				moveDir;						//進行方向
	int						state;

}ENERGYITEM;

enum ENEGYITEM_STATE
{
	ENERGYITEM_DEFAULT,
	ENERGYITEM_REDUCTION,
	ENERGYITEM_STATEMAX
};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnergyItem(int num);							//エナジーアイテム初期化処理
void UninitEnergyItem(void);								//エナジーアイテム終了処理
void UpdateEnergyItem(void);								//エナジーアイテム更新処理
void DrawEnergyItem(void);									//エナジーアイテム描画処理
ENERGYITEM *GetEnergyItemAdr(int pno);						//エナジーアイテムアドレス取得関数
void SetEnergyItem(D3DXVECTOR3 pos);						//エナジーアイテムセット関数

#endif
