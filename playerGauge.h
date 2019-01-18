//=============================================================================
//
// プレイヤーゲージヘッダ [PlayerGauge.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _PLAYERGAUGE_H_
#define _PLAYERGAUGE_H_


// マクロ定義
#define PLAYERGAUGE_TEXTURE_NAME	_T("data/TEXTURE/player/circuit3.png")	// プレイヤーゲージのテクスチャ
#define PLAYERGAUGE_BACKTEXTURE_NAME _T("data/TEXTURE/player/circuit2.png")	// プレイヤーゲージ（バック）のテクスチャ

#define PLAYERGAUGE_TEXTURE_SIZE_X					(150 / 2)					// テクスチャサイズX
#define PLAYERGAUGE_TEXTURE_SIZE_Y					(150 / 2)					// テクスチャサイズY

#define PLAYERGAUGE_MAX								(1)							//プレイヤーゲージの最大数

#define PLAYERGAUGE_INITPOS_X						(-100)						//プレイヤーゲージの初期X座標
#define PLAYERGAUGE_INITPOS_Y						(-100)						//プレイヤーゲージの初期Y座標

#define PLAYERGAUGE_VERTEX_NUM						(144 + 2)					//プレイヤーゲージの頂点数
//*****************************************************************************
// 構造体定義
//*****************************************************************************
enum GAUGESTATE
{
	GAUGESTATE_OPEN,
	GAUGESTATE_NORMAL,
	GAUGESTATE_CLOSE
};

typedef struct
{
	bool					active;						//使用or未使用の判定

	D3DXVECTOR3				pos;						// ポリゴンの座標
	D3DXVECTOR3				rot;						// ポリゴンの回転量

	float					radius;						// ポリゴンの半径
	float					angle;						// ポリゴンの角度

	float					percent;					//ゲージの円周の長さを決めるメンバ
	float					scale;						//ゲージの大きさ

	GAUGESTATE				state;						//ゲージの状態
}PLAYERGAUGE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayerGauge(int num);						//プレイヤーゲージ初期化処理
void UninitPlayerGauge(void);							//プレイヤーゲージ終了処理
void UpdatePlayerGauge(void);							//プレイヤーゲージ更新処理
void DrawPlayerGauge(void);								//プレイヤーゲージ描画処理
PLAYERGAUGE *GetPlayerGaugeAdr(int pno);				//プレイヤーゲージのアドレス取得関数
void SetGaugeState(GAUGESTATE state, int no);			//プレイヤーゲージの状態セット関数

#endif