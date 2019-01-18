//=============================================================================
//
// エネミーマネージャ処理 [enemyManager.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "particle.h"
#include "scoreItem.h"
#include "enemyManager.h"
#include "sePlayer.h"
#include "GUI.h"
#include "stageManager.h"
#include "enemyParticle.h"

//#include "testEnemy.h"
//#include "bigEnemy1.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define BIGEXPLOSION_RANGE			(100.0f)				//大型エネミーの爆発が発生する範囲
#define BIGEXPLOSION_NUM			(5)						//大型エネミーの爆発で発生する爆炎の数
#define BIGEXPLOSION_ITEMNUM		(20)					//大型エネミーの爆発で発生するアイテムの数

#define MIDDLEEXPLOSION_ITEMNUM		(5)						//中型エネミーの爆発で発生するアイテムの数
#define MIDDLEEXPLOSION_RANGE		(70.0f)					//中型エネミーの爆発で発生するアイテムの範囲

#define SMALLEXPLOSION_ITEMNUM		(1)						//小型エネミーの爆発で発生するアイテムの数

#define ENEMYEXPLOSION_EMITPARTICLE_NUM		(3)
#define MIDDLEEXPLOSION_EMITPARTICLE_NUM	(10)
#define BIGEXPLOSION_EMITPARTICLE_NUM		(500)

#define ENEMYPARTICLE_EMITTER_DURATION	(5)
/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/


/*****************************************************************************
グローバル変数
*****************************************************************************/


/******************************************************************************
初期化処理
*****************************************************************************/
HRESULT InitEnemyManager(int num)
{
	InitEnemyGauge(num);
	InitBossGauge(num);
	InitTestEnemy(num);
	InitBigEnemy1(num);
	InitSmallPropeller(num);
	InitMiddleTank(num);
	InitSmallTank(num);
	InitSmallZako1(num);
	InitSmallZako2(num);
	InitItemCareer(num);
	InitStage1Boss(num);
	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitEnemyManager(void)
{
	UninitTestEnemy();
	UninitBigEnemy1();
	UninitEnemyGauge();
	UninitSmallPropeller();
	UninitMiddleTank();
	UninitSmallTank();
	UninitSmallZako1();
	UninitSmallZako2();
	UninitItemCareer();
	UninitStage1Boss();
	UninitBossGauge();
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateEnemyManager(void)
{
	//エネミーの更新
	UpdateTestEnemy();
	UpdateBigEnemy1();
	UpdateSmallPropeller();
	UpdateMiddleTank();
	UpdateSmallTank();
	UpdateSmallZako1();
	UpdateSmallZako2();
	UpdateItemCareer();
	UpdateStage1Boss();

	//エネミーゲージの更新
	UpdateEnemyGauge();
	UpdateBossGauge();
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawEnemyManager(void)
{
	//地上エネミーの描画
	DrawSmallTank();
	DrawMiddleTank();

	//大型エネミーの描画
	DrawStage1Boss();
	DrawBigEnemy1();

	//中型エネミーの描画
	DrawTestEnemy();
	DrawItemCareer();

	//小型エネミーの描画
	DrawSmallPropeller();
	DrawSmallZako1();
	DrawSmallZako2();
	
}

/******************************************************************************
//関数名	：void ExplodeBigEnemy(D3DXVECTOR3 pos)
//引数1		：D3DXVECTOR3 pos	：爆発をセットする座標
//戻り値	：void
//説明		：大型エネミーの爆発をセットする関数
******************************************************************************/
void ExplodeBigEnemy(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 tmp;
	for (int i = 0; i < BIGEXPLOSION_NUM; i++)
	{
		tmp = pos;
		tmp.x += RandomRange(-BIGEXPLOSION_RANGE, BIGEXPLOSION_RANGE);
		tmp.y += RandomRange(-BIGEXPLOSION_RANGE, BIGEXPLOSION_RANGE);
		SetParticle(tmp, EXPLOSION);
	}

	SetScoreItem(pos, BIGEXPLOSION_RANGE, BIGEXPLOSION_ITEMNUM);
	SetParticle(pos, PIECE);
	SetEmitterEnemyParticle(pos, ENEMYPARTICLE_EMITTER_DURATION, BIGEXPLOSION_EMITPARTICLE_NUM ,EMITTER_CIRCLE);
	SetGUIShake();
	AddComboCount(1);
	PlaySE(SE_BIGEXP);
}

/******************************************************************************
//関数名	：void ExplodeMiddleEnemy(D3DXVECTOR3 pos)
//引数1		：D3DXVECTOR3 pos	：爆発をセットする座標
//戻り値	：void
//説明		：中型エネミーの爆発をセットする関数
******************************************************************************/
void ExplodeMiddleEnemy(D3DXVECTOR3 pos)
{
	SetParticle(pos, EXPLOSION);
	SetScoreItem(pos, MIDDLEEXPLOSION_RANGE, MIDDLEEXPLOSION_ITEMNUM);
	SetParticle(pos, PIECE);
	AddComboCount(1);
	SetEmitterEnemyParticle(pos, ENEMYPARTICLE_EMITTER_DURATION, MIDDLEEXPLOSION_EMITPARTICLE_NUM, EMITTER_CONE);
	PlaySE(SE_MIDDLEEXP);
}

/******************************************************************************
//関数名	：void ExplodeSmallEnemy(D3DXVECTOR3 pos)
//引数1		：D3DXVECTOR3 pos	：爆発をセットする座標
//戻り値	：void
//説明		：小型エネミーの爆発をセットする関数
******************************************************************************/
void ExplodeSmallEnemy(D3DXVECTOR3 pos)
{
	SetParticle(pos, FIRE);
	SetParticle(pos, PIECE);
	SetScoreItem(pos, 0.0f, SMALLEXPLOSION_ITEMNUM);
	AddComboCount(1);
	SetEmitterEnemyParticle(pos, ENEMYPARTICLE_EMITTER_DURATION, ENEMYEXPLOSION_EMITPARTICLE_NUM, EMITTER_CONE);
	PlaySE(SE_SMALLEXP);
}

/******************************************************************************
//関数名	：void SetTextureEnemyShadow(VERTEX_2D *vertexWk)
//引数1		：VERTEX_2D *vertexWk	：操作する頂点情報
//戻り値	：void
//説明		：エネミーの影用にテクスチャ座標を設定する関数
******************************************************************************/
void SetTextureEnemyShadow(VERTEX_2D *vertexWk)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}