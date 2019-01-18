//=============================================================================
//
// プレイヤーボンバーヘッダ [PlayerBomber.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _PLAYERBOMBER_H_
#define _PLAYERBOMBER_H_


// マクロ定義
#define PLAYERBOMBER_TEXTURE_NAME	_T("data/TEXTURE/player/playerBomber.png")	// プレイヤーボンバーのテクスチャ

#define PLAYERBOMBER_TEXTURE_SIZE_X					(300)					// テクスチャサイズX
#define PLAYERBOMBER_TEXTURE_SIZE_Y					(300)					// テクスチャサイズY

#define PLAYERBOMBER_TEXTURE_DIVIDE_X				(1)						// アニメパターンのテクスチャ内分割数（X)
#define PLAYERBOMBER_TEXTURE_DIVIDE_Y				(1)						// アニメパターンのテクスチャ内分割数（Y)
#define PLAYERBOMBER_ANIM_NUM			(PLAYERBOMBER_TEXTURE_DIVIDE_X*PLAYERBOMBER_TEXTURE_DIVIDE_Y)	// アニメーションパターン数
#define PLAYERBOMBER_TIME_ANIMATION					(4)						// アニメーションの切り替わるカウント

#define PLAYERBOMBER_MAX							(1)					//プレイヤーボンバーの最大数

#define PLAYERBOMBER_INITPOS_X						(-100)					//プレイヤーボンバーの初期X座標
#define PLAYERBOMBER_INITPOS_Y						(-100)					//プレイヤーボンバーの初期Y座標

//プレイヤーボンバーのバウンディングボックスの定義
#define PLAYERBOMBER_BB_TOP							(0.0f)
#define PLAYERBOMBER_BB_LEFT						(0.0f)
#define PLAYERBOMBER_BB_WIDTH						(250.0f)
#define PLAYERBOMBER_BB_HIGHT						(250.0f)

//プレイヤーボンバーのバウンディングサークル定義
#define PLAYERBOMBER_BC_RADIUS						(250)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	bool					active;							//使用or未使用の判定

	D3DXVECTOR3				pos;						// ポリゴンの座標
	D3DXVECTOR3				rot;						// ポリゴンの回転量
	int						countAnim;						// アニメーションカウント
	int						patternAnim;					// アニメーションパターンナンバー

	float					radius;							// ポリゴンの半径
	float					angle;						// ポリゴンの角度
	float					scale;					//ポリゴンのスケール

}PLAYERBOMBER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayerBomber(int num);							//プレイヤーボンバー初期化処理
void UninitPlayerBomber(void);								//プレイヤーボンバー終了処理
void UpdatePlayerBomber(void);								//プレイヤーボンバー更新処理
void DrawPlayerBomber(void);								//プレイヤーボンバー描画処理
PLAYERBOMBER *GetPlayerBomberAdr(int pno);						//プレイヤーボンバーアドレス取得関数
bool SetPlayerBomber(D3DXVECTOR3 pos);						//プレイヤーボンバーセット関数
#endif
