//=============================================================================
//
// エネミーマネージャヘッダ [EnemyManager.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _ENEMYMANAGER_H_
#define _ENEMYMANAGER_H_

#include "enemyGauge.h"
#include "bossGauge.h"
#include "bg.h"
#include "Score.h"
#include "enemyBullet.h"
#include "testEnemy.h"
#include "bigEnemy1.h"
#include "smallPropeller.h"
#include "middleTank.h"
#include "smallTank.h"
#include "smallZako1.h"
#include "itemCareer.h"
#include "smallZako2.h"
#include "stage1Boss.h"

// マクロ定義

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemyManager(int num);							//エネミーマネージャ初期化処理
void UninitEnemyManager(void);								//エネミーマネージャ終了処理
void UpdateEnemyManager(void);								//エネミーマネージャ更新処理
void DrawEnemyManager(void);								//エネミーマネージャ描画処理
void ExplodeBigEnemy(D3DXVECTOR3 pos);						//大型エネミーの爆発セット関数
void ExplodeMiddleEnemy(D3DXVECTOR3 pos);					//中型エネミーの爆発セット関数
void ExplodeSmallEnemy(D3DXVECTOR3 pos);					//小型エネミーの爆発セット関数
void SetTextureEnemyShadow(VERTEX_2D *vertexWk);			//エネミーの影テクスチャ座標設定関数
#endif
