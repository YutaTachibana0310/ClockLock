//=============================================================================
//
// Mainヘッダー [main.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止

#include "windows.h"
#include <time.h>
#include "d3dx9.h"
#include <tchar.h>


#define DIRECTINPUT_VERSION 0x0800		// 警告対処
#include "dinput.h"
#include "mmsystem.h"

#if 1	// [ここを"0"にした場合、"構成プロパティ" -> "リンカ" -> "入力" -> "追加の依存ファイル"に対象ライブラリを設定する]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#endif

#define SCREEN_WIDTH	(1280)												// ウインドウの幅4
#define SCREEN_HEIGHT	(960)												// ウインドウの高さ3
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)									// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)									// ウインドウの中心Ｙ座標
#define SCREEN_END_LEFT	(BG_POS_X)
#define SCREEN_END_RIGHT (BG_POS_X + BG_TEXTURESIZE_X)

#define	NUM_VERTEX		(4)													// 頂点数
#define NUM_POLYGON		(2)

#define VECTOR3_UP		(D3DXVECTOR3(0.0f, -1.0f, 0.0f))					//画面上方向への単位ベクトル
#define VECTOR3_RIGHT	(D3DXVECTOR3(1.0f, 0.0f, 0.0f))						//画面右方向への単位ベクトル
#define VECTOR3_ZERO	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))						//0ベクトル

#define PLAYERNAME_MAX	(5)													//プレイヤー名文字最大数

#ifdef _DEBUG
#define COLLIDER_COLOR		(D3DCOLOR_RGBA(255, 0, 0, 255))					//当たり判定の描画色
#define NUM_BB_VERTEX	(5)													//バウンディングボックスを描画する際の頂点数
#endif

// 頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//角度をラジアンに変換する引数付きマクロ(xが角度）
#define RADIAN(x)					((x) * 0.01745329f)				

// 上記頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	float rhw;				// テクスチャのパースペクティブコレクト用
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_2D;

enum DEFINE_SCENE
{
	SCENE_TITLE,
	SCENE_TUTORIAL,
	SCENE_GAME,
	SCENE_PAUSE,
	SCENE_CLEAR,
	SCENE_GAMEOVER,
	SCENE_NAMEENTRY
};

enum DEFINE_DIFFICULTY
{
	DIFFICULTY_EASY,
	DIFFICULTY_NORMAL,
	DIFFICULTY_HARD,
	DIFFICULTY_MAX
};

enum EMITTER_SHAPE
{
	EMITTER_CONE,
	EMITTER_CIRCLE,
	EMITTER_SHAPEMAX
};

#include "myLibrary.h"

/*******************************************************
プロトタイプ宣言
********************************************************/
LPDIRECT3DDEVICE9	GetDevice(void);				// デバイス取得関数
void Pause(bool state);								//ポーズ切り替え関数
void SetScene(int scene);							//シーン切り替え関数
void InitGame(int stage);							//ステージ開始前の初期化処理								
int GetDifficulty(void);							//難易度取得関数
void SetDifficulty(int n);							//難易度設定関数
void ResumeGame(void);								//ゲーム再開関数
int GetCurrentScene(void);							//シーン取得関数
bool isDemoMode(void);								//デモモード判定関数

#ifdef _DEBUG
void DrawBoxCollider(D3DXVECTOR3 basePos, float width, float hight);					//バウンディングボックスを可視化する関数
#endif

#endif