//=============================================================================
//
// プレイヤーヘッダー [Player.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "bulletFire.h"

// マクロ定義
#define PLAYER_TEXTURE	_T("data/TEXTURE/player/player.png")				// プレイヤーのテクスチャ
#define PLAYER_CORETEX_NAME	_T("data/TEXTURE/player/playercore.png")		// プレイヤーコアのテクスチャ
#define PLAYER_TEXTURE_SIZE_X		(90)									// テクスチャサイズ（X)
#define PLAYER_TEXTURE_SIZE_Y		(120)									// テクスチャサイズ（Y)

#define TEXTURE_PATTERN_DIVIDE_X	(4)										// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(3)										// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define TIME_ANIMATION				(4)										// アニメーションの切り替わるカウント

#define PLAYER_MAX					(1)										//プレイヤーの最大数

#define PLAYER_INITPOS_X			(SCREEN_CENTER_X)						//プレイヤーの初期座標
#define PLAYER_INITPOS_Y			(SCREEN_HEIGHT * 1.1f)					//プレイヤーの初期座標

#define PLAYER_NUM_BULLETFIRE		(4)										//バレット発射のエフェクトの数

#define PLAYER_BB_LEFT				(0.0f)									//プレイヤーの当たり判定（左からのオフセット値）
#define PLAYER_BB_WIDTH				(5.0f)									//プレイヤーの当たり判定（幅）
#define PLAYER_BB_TOP				(-10.0f)								//プレイヤーの当たり判定（上からのオフセット値）
#define PLAYER_BB_HIGHT				(5.0f)									//プレイヤーの当たり判定（高さ）

#define PLAYER_ITEMBB_WIDTH			(20.0f)									//プレイヤーのアイテム用当たり判定（幅）
#define PLAYER_ITEMBB_HIGHT			(20.0f)									//プレイヤーのアイテム用当たり判定（高さ）

#define PLAYER_TIMEEFFECT_MAX		(720.0f)								//時間操作のMAX

#define PLAYER_CORE_SIZE_X			(10)									//プレイヤーコアのテクスチャサイズ
#define PLAYER_CORE_SIZE_Y			(10)									//プレイヤーコアのテクスチャサイズ
#define PLAYER_CORE_OFFSET_Y		(-10.0f)								//プレイヤーコアのオフセット座標
#define PLAYER_CORE_DIVIDE_X		(6)										//プレイヤーコアのテクスチャ分割数X
#define PLAYER_CORE_DIVIDE_Y		(1)										//プレイヤーコアのテクスチャ分割数Y
#define PLAYER_CORE_PATTERNMAX		(6)										//プレイヤーコアのアニメーションMAX

#define PLAYER_TRAILPOS_NUM			(50)									//保存するトレイルの数
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	bool			active;								//アクティブ状態
	bool			flgInput;							//入力の受付状態

	int				shotInterbal;						//最後にショットを撃ってからの経過フレーム
	int				numFireShot;						//サブショットの発射回数
	int				countAnim;							// アニメーションカウント
	int				patternAnim;						// アニメーションパターンナンバー
	int				playerDir;							//プレイヤーのX軸進行方向（テクスチャ設定用）
	float			timeEffect;							//時間操作エフェクトの残りフレーム
	int				zanki;								//残機

	float			angle;								//頂点への角度
	float			radius;								//ポリゴンの半径

	float			angleCore;							//コアのポリゴン角度
	float			radiusCore;							//コアのポリゴン半径

	D3DXVECTOR3		pos;								//座標
	D3DXVECTOR3		rot;								//回転量
	BULLETFIRE		*fire[PLAYER_NUM_BULLETFIRE];		//バレット発射のエフェクト
	VERTEX_2D		vertexWk[NUM_VERTEX];				//頂点情報格納ワーク
	VERTEX_2D		shadowWk[NUM_VERTEX];				//影用の頂点情報

	int				state;								//プレイヤーの状態
	int				bomCnt;								//ボム残弾

	float			gaugeDeltaValue;					//1フレームのゲージ減少量

	bool			lockTimeEffect;						//時間操作機能のロック
	bool			lockBomber;							//ボンバーのロック

	int				corePattern;						//コアのアニメーションパターン
	int				cntFrame;							//カウントフレーム

	D3DXVECTOR3		trailPos[PLAYER_TRAILPOS_NUM];		//トレイル

}PLAYER;

//プレイヤーの状態列挙型
enum PLAYER_STATE
{
	PLAYER_INITIALIZED,
	PLAYER_INVINCIBLE,
	PLAYER_DEFAULT,
	PLAYER_DESTROYED,
	PLAYER_GAMEOVER
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(int num);						//プレイヤー初期化処理
void UninitPlayer(void);							//プレイヤー終了処理
void UpdatePlayer(void);							//プレイヤー更新処理
void DrawPlayer(void);								//プレイヤー描画処理
PLAYER *GetPlayerAdr(int pno);						//プレイヤーアドレス取得関数
void DestroyPlayer(int i);							//プレイヤー撃墜処理
void RespornPlayer(int i);							//プレイヤー再出撃処理

#endif
