//=============================================================================
//
// プレイヤーバレットヘッダ [PlayerBullet.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _PLAYERBULLET_H_
#define _PLAYERBULLET_H_


// マクロ定義
#define PLAYERBULLET_TEXTURE_MAIN	_T("data/TEXTURE/player/playerbullet.png")	// プレイヤーバレットのテクスチャ
#define PLAYERBULLET_TEXTURE_SUB	_T("data/TEXTURE/player/playerbullet.png")	// プレイヤーバレットのテクスチャ

#define PLAYERBULLET_TEXTURE_SIZE_X					(70 / 2)					// テクスチャサイズX
#define PLAYERBULLET_TEXTURE_SIZE_Y					(80 / 2)					// テクスチャサイズY

#define PLAYERBULLET_CENTER_X						(35)					//プレイヤーバレットの中心位置X
#define PLAYERBULLET_CENTER_Y						(40)					//プレイヤーバレットの中心位置Y

#define PLAYERBULLET_TEXTURE_DIVIDE_X				(1)						// アニメパターンのテクスチャ内分割数（X)
#define PLAYERBULLET_TEXTURE_DIVIDE_Y				(1)						// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PLAYERBULLET_NUM			(PLAYERBULLET_TEXTURE_DIVIDE_X*PLAYERBULLET_TEXTURE_DIVIDE_Y)	// アニメーションパターン数
#define TIMEPLAYERBULLET_ANIMATION					(4)						// アニメーションの切り替わるカウント

#define PLAYERBULLET_MAIN_MAX						(56)					//プレイヤーバレットの最大数（メインショット）
#define PLAYERBULLET_SUB_MAX						(128)					//プレイヤーバレットの最大数（サブショット）

#define PLAYERBULLET_INITPOS_X						(-100)					//プレイヤーバレットの初期X座標
#define PLAYERBULLET_INITPOS_Y						(-100)					//プレイヤーバレットの初期Y座標

//プレイヤーバレットのバウンディングボックスの定義
#define PLAYERBULLET_BB_TOP							(0.0f)
#define PLAYERBULLET_BB_LEFT						(0.0f)
#define PLAYERBULLET_BB_WIDTH						(10.0f)
#define PLAYERBULLET_BB_HIGHT						(35.0f)

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

	D3DXVECTOR3				moveDir;						//進行方向

}PLAYERBULLET;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayerBullet(int num);								//プレイヤーバレット初期化処理
void UninitPlayerBullet(void);									//プレイヤーバレット終了処理
void UpdatePlayerBullet(void);									//プレイヤーバレット更新処理
void DrawPlayerBullet(void);									//プレイヤーバレット描画処理
PLAYERBULLET *GetPlayerBulletAdr(int pno);						//プレイヤーバレットアドレス取得関数（メインショット）
PLAYERBULLET *GetPlayerSubBulletAdr(int pno);					//プレイヤーバレットアドレス取得関数（サブショット）
void FirePlayerBullet(D3DXVECTOR3 pos);							//プレイヤーバレットセット関数（メインショット）
void FirePlayerSubBullet(D3DXVECTOR3 pos, float rotAngle);		//プレイヤーバレットセット関数（サブショット）


#endif
