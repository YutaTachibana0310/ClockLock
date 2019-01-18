//=============================================================================
//
// コライダー処理 [collider.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "collider.h"
#include "player.h"
#include "playerBullet.h"
#include "enemyBullet.h"
#include "PBulletExplosion.h"
#include "scoreItem.h"
#include "Score.h"
#include "enemyManager.h"
#include "sePlayer.h"
#include "energyItem.h"
#include "playerBomber.h"
#include "tutorialEnemy.h"
#include "playerShield.h"
#include "EnemyBulletExplosion.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define PLAYER_ENERGY_ADDVALUEEASY			(2.0f)		//アイテム取得時に増えるエナジーの量（EASY)
#define PLAYER_ENERGY_ADDVALUENORMAL		(1.5f)		//アイテム取得時に増えるエナジーの量（NORMAL）
#define PLAYER_ENERGY_ADDVALUEHARD			(0.5f)		//アイテム取得時に増えるエナジーの量（HARD)

#define BOMBER_DAMAGE						(3)			//ボンバーのダメージ

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
void CollisionEnemyBullet(void);					//エネミーバレットの当たり判定関数
void CollisionTestEnemy(void);						//テストエネミーの当たり判定関数
void CollisionScoreItem(void);						//スコアアイテムの当たり判定
void CollisionBigEnemy1(void);						//ビッグエネミー1の当たり判定
void CollisionSmallPropeller();						//スモールプロペラと接触判定
void CollisionMiddleTank(void);						//ミドルタンクの当たり判定
void CollisionSmallTank(void);						//スモールタンクの当たり判定
void CollisionSmallZako1(void);						//スモールザコ1の当たり判定
void CollisionSmallZako2(void);						//スモールザコ2の当たり判定
void CollisionItemCareer(void);						//アイテムキャリアの当たり判定
void CollisionEnergyItem(void);						//エナジーアイテムの当たり判定
void CollisionStage1Boss(void);						//ステージ1ボスの当たり判定
void CollisionTutorialEnemy(void);					//チュートリアルエネミーの当たり判定
void CollisionBomber(void);							//ボンバーの当たり判定
void CollisionTutorialEnemy(void);					//チュートリアルエネミーの当たり判定
void CollisionPlayerShield(void);					//プレイヤーシールドの当たり判定

/*****************************************************************************
グローバル変数
*****************************************************************************/

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitCollider(void)
{
	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitCollider(void)
{

}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateCollider(void)
{
	//スコアアイテムの当たり判定
	CollisionScoreItem();
	CollisionEnergyItem();

	//エネミーの当たり判定
	CollisionTestEnemy();
	CollisionBigEnemy1();
	CollisionSmallPropeller();
	CollisionMiddleTank();
	CollisionSmallTank();
	CollisionSmallZako1();
	CollisionSmallZako2();
	CollisionItemCareer();
	CollisionStage1Boss();
	CollisionTutorialEnemy();
	//CollisionPlayerShield();
	CollisionBomber();
	CollisionPlayerShield();

	//エネミーバレットの当たり判定
	CollisionEnemyBullet();
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawCollider(void)
{

}

/******************************************************************************
//関数名	：bool HitBB(D3DXVECTOR4 bb1, D3DXVECTOR4 bb2)
//引数1		：D3DXVECTOR4 bb1	：バウンディングボックス1（x:x座標 y:y座標 z:ボックス幅(半分) w:ボックス高さ(半分)）
//引数2		：D3DXVECTOR4 bb2	：バウンディングボックス1（x:x座標 y:y座標 z:ボックス幅(半分) w:ボックス高さ(半分)）
//戻り値	：2つのBBが接触している場合：true	接触していない場合：false
//説明		：バウンディングボックスの接触を判定する関数
******************************************************************************/
bool HitBB(D3DXVECTOR4 bb1, D3DXVECTOR4 bb2)
{
	if (bb1.x + bb1.z > bb2.x - bb2.z && bb1.x - bb1.z < bb2.x + bb2.z)
	{
		if (bb1.y + bb1.w > bb2.y - bb2.w && bb1.y - bb1.w < bb2.y + bb2.w)
		{
			return true;
		}
	}
	return false;
}

/******************************************************************************
//関数名	：bool HitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float radius1, float radius2)
//引数1		：D3DXVRCTOR3 pos1		：対象1の座標
//引数2		：D3DXVRCTOR3 pos1		：対象2の座標
//引数3		：float radius1			：対象1の半径
//引数4		：float radius2			：対象2の半径
//戻り値	：2つのBCが接触している場合：true	接触していない場合：false
//説明		：バウンディングサークルの接触を判定する関数
******************************************************************************/
bool HitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float radius1, float radius2)
{
	float lengthSq = D3DXVec3LengthSq(&(pos1 - pos2));

	return (pow(radius1 + radius2, 2) > lengthSq) ? true : false;

}

/******************************************************************************
//関数名	：void CollisionBullet(void)
//引数1		：void
//戻り値	：void
//説明		：ネミーバレットの接触判定
******************************************************************************/
void CollisionEnemyBullet(void) {

	int i, j;
	PLAYER *player = GetPlayerAdr(0);								//プレイヤーへのポインタ
	ENEMYBULLET *eBullet = GetEnemyBulletAdr(GREEN_ROTATE, 0);		//エネミーバレットのポインタ
	D3DXVECTOR4 bb1, bb2;											//プレイヤー、エネミーバレットのバウンディングボックス

	//全てのプレイヤーとエネミーバレットで接触判定
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->state != PLAYER_DEFAULT)
		{
			continue;
		}

		eBullet = GetEnemyBulletAdr(GREEN_ROTATE, 0);
		for (j = 0; j < ENEMYBULLET_MAX; j++, eBullet++)
		{
			if (!eBullet->active)
			{
				continue;
			}


			bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_BB_WIDTH, PLAYER_BB_HIGHT);
			bb2 = D3DXVECTOR4(eBullet->pos.x + ENEMYBULLET_BB_LEFT, eBullet->pos.y + ENEMYBULLET_BB_TOP, ENEMYBULLET_BB_WIDTH, ENEMYBULLET_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				//プレイヤーとエネミーバレットが接触していたら撃墜
				DestroyPlayer(i);
			}
		}
	}
}

/******************************************************************************
//関数名	：void CollisionTestEnemy(void)
//引数1		：void
//戻り値	：void
//説明		：テストエネミーの接触判定
******************************************************************************/
void CollisionTestEnemy(void)
{
	int i, j;
	PLAYERBULLET *pBullet = GetPlayerBulletAdr(0);
	TESTENEMY *enemy = GetTestEnemyAdr(0);
	D3DXVECTOR4 bb1, bb2;

	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetTestEnemyAdr(0);
		for (j = 0; j < TESTENEMY_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + TESTENEMY_BB_LEFT, enemy->pos.y + TESTENEMY_BB_TOP, TESTENEMY_BB_WIDTH, TESTENEMY_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	pBullet = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetTestEnemyAdr(0);
		for (j = 0; j < TESTENEMY_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + TESTENEMY_BB_LEFT, enemy->pos.y + TESTENEMY_BB_TOP, TESTENEMY_BB_WIDTH, TESTENEMY_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}
}

/******************************************************************************
//関数名	：void CollisionTestEnemy(void)
//引数1		：void
//戻り値	：void
//説明		：スコアアイテムの接触判定
******************************************************************************/
void CollisionScoreItem(void)
{
	PLAYER *player = GetPlayerAdr(0);
	SCOREITEM *item = GetScoreItemAdr(0);
	D3DXVECTOR4 bb1, bb2;
	int i, j, difficulty = GetDifficulty();
	float addValue;

	addValue = (difficulty == DIFFICULTY_EASY) ? PLAYER_ENERGY_ADDVALUEEASY : (difficulty == DIFFICULTY_NORMAL) ? PLAYER_ENERGY_ADDVALUENORMAL : PLAYER_ENERGY_ADDVALUEHARD;
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (!player->active)
		{
			continue;
		}

		for (j = 0; j < SCOREITEM_MAX; j++, item++)
		{
			if (!item->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_ITEMBB_WIDTH, PLAYER_ITEMBB_HIGHT);
			bb2 = D3DXVECTOR4(item->pos.x + SCOREITEM_BB_LEFT, item->pos.y + SCOREITEM_BB_TOP, SCOREITEM_BB_WIDTH, SCOREITEM_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				//スコアを加算しアイテムを非アクティブにセット
				AddScore(SCOREITEM_ADDVALUE);
				SetScoreItemCount(1);
				item->active = false;
				player->timeEffect = Clampf(0.0f, PLAYER_TIMEEFFECT_MAX, player->timeEffect + addValue);
				PlaySE(SE_GETITEM);
#ifdef _DEBUG
				SetActiveItemCount(-1);
#endif
			}
		}
	}
}

/******************************************************************************
//関数名	：void CollisionEnergyEnemy(void)
//引数1		：void
//戻り値	：void
//説明		：エナジーアイテムの接触判定
******************************************************************************/
void CollisionEnergyItem(void)
{
	PLAYER *player = GetPlayerAdr(0);
	ENERGYITEM *item = GetEnergyItemAdr(0);
	D3DXVECTOR4 bb1, bb2;
	int i, j;

	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (!player->active)
		{
			continue;
		}

		for (j = 0; j < ENERGYITEM_MAX; j++, item++)
		{
			if (!item->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_ITEMBB_WIDTH, PLAYER_ITEMBB_HIGHT);
			bb2 = D3DXVECTOR4(item->pos.x + ENERGYITEM_BB_LEFT, item->pos.y + ENERGYITEM_BB_TOP, ENERGYITEM_BB_WIDTH, ENERGYITEM_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				//エナジーを加算しアイテムを非アクティブにセット
				player->timeEffect = Clampf(0, PLAYER_TIMEEFFECT_MAX, player->timeEffect + ENERGYITEM_ADDVALUE);
				item->active = false;
				PlaySE(SE_GETITEM);
			}
		}
	}
}
/******************************************************************************
//関数名	：void CollisionBigEnemy1(void)
//引数1		：void
//戻り値	：void
//説明		：ビッグエネミー1の接触判定
******************************************************************************/
void CollisionBigEnemy1(void)
{
	int i, j;
	PLAYERBULLET *pBullet = GetPlayerBulletAdr(0);
	BIGENEMY1 *enemy = GetBigEnemy1Adr(0);
	D3DXVECTOR4 bb1, bb2;


	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetBigEnemy1Adr(0);
		for (j = 0; j < BIGENEMY1_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + BIGENEMY1_BB_LEFT, enemy->pos.y + BIGENEMY1_BB_TOP, BIGENEMY1_BB_WIDTH, BIGENEMY1_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	pBullet = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetBigEnemy1Adr(0);
		for (j = 0; j < BIGENEMY1_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + BIGENEMY1_BB_LEFT, enemy->pos.y + BIGENEMY1_BB_TOP, BIGENEMY1_BB_WIDTH, BIGENEMY1_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}


	//ボムとの当たり判定
	PLAYERBOMBER *bom = GetPlayerBomberAdr(0);
	for (i = 0; i < PLAYERBOMBER_MAX; i++, bom++)
	{
		if (!bom->active)
		{
			continue;
		}

		enemy = GetBigEnemy1Adr(0);
		for (j = 0; j < BIGENEMY1_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(bom->pos.x + PLAYERBOMBER_BB_LEFT, bom->pos.y + PLAYERBOMBER_BB_TOP, PLAYERBOMBER_BB_WIDTH * bom->scale, PLAYERBOMBER_BB_HIGHT * bom->scale);
			bb2 = D3DXVECTOR4(enemy->pos.x + BIGENEMY1_BB_LEFT, enemy->pos.y + BIGENEMY1_BB_TOP, BIGENEMY1_BB_WIDTH, BIGENEMY1_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= BOMBER_DAMAGE;
			}
		}
	}

	//プレイヤーとの当たり判定
	PLAYER *player = GetPlayerAdr(0);
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->state != PLAYER_DEFAULT)
		{
			continue;
		}

		enemy = GetBigEnemy1Adr(0);
		for (j = 0; j < BIGENEMY1_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_BB_WIDTH, PLAYER_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + BIGENEMY1_BB_LEFT, enemy->pos.y + BIGENEMY1_BB_TOP, BIGENEMY1_BB_WIDTH, BIGENEMY1_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				DestroyPlayer(i);
			}
		}
	}
}

/******************************************************************************
//関数名	：void CollisionSmallPropeller(void)
//引数1		：void
//戻り値	：void
//説明		：スモールプロペラの接触判定
******************************************************************************/
void CollisionSmallPropeller(void)
{
	int i, j;
	PLAYERBULLET *pBullet = GetPlayerBulletAdr(0);
	SMALLPROPELLER *enemy = GetSmallPropellerAdr(0);
	D3DXVECTOR4 bb1, bb2;

	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetSmallPropellerAdr(0);
		for (j = 0; j < SMALLPROPELLER_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLPROPELLER_BB_LEFT, enemy->pos.y + SMALLPROPELLER_BB_TOP, SMALLPROPELLER_BB_WIDTH, SMALLPROPELLER_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	pBullet = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetSmallPropellerAdr(0);
		for (j = 0; j < SMALLPROPELLER_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLPROPELLER_BB_LEFT, enemy->pos.y + SMALLPROPELLER_BB_TOP, SMALLPROPELLER_BB_WIDTH, SMALLPROPELLER_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	//ボムとの当たり判定
	PLAYERBOMBER *bom = GetPlayerBomberAdr(0);
	for (i = 0; i < PLAYERBOMBER_MAX; i++, bom++)
	{
		if (!bom->active)
		{
			continue;
		}
		enemy = GetSmallPropellerAdr(0);
		for (j = 0; j < SMALLPROPELLER_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(bom->pos.x + PLAYERBOMBER_BB_LEFT, bom->pos.y + PLAYERBOMBER_BB_TOP, PLAYERBOMBER_BB_WIDTH * bom->scale, PLAYERBOMBER_BB_HIGHT * bom->scale);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLPROPELLER_BB_LEFT, enemy->pos.y + SMALLPROPELLER_BB_TOP, SMALLPROPELLER_BB_WIDTH, SMALLPROPELLER_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= BOMBER_DAMAGE;
			}
		}
	}

	//プレイヤーとの当たり判定
	PLAYER *player = GetPlayerAdr(0);
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->state != PLAYER_DEFAULT)
		{
			continue;
		}

		enemy = GetSmallPropellerAdr(0);
		for (j = 0; j < BIGENEMY1_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_BB_WIDTH, PLAYER_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLPROPELLER_BB_LEFT, enemy->pos.y + SMALLPROPELLER_BB_TOP, SMALLPROPELLER_BB_WIDTH, SMALLPROPELLER_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				DestroyPlayer(i);
			}
		}
	}
}

/******************************************************************************
//関数名	：void CollisionMiddleTank(void)
//引数1		：void
//戻り値	：void
//説明		：ミドルタンクの接触判定
******************************************************************************/
void CollisionMiddleTank(void)
{
	int i, j;
	PLAYERBULLET *pBullet = GetPlayerBulletAdr(0);
	MIDDLETANK *enemy = GetMiddleTankAdr(0);
	D3DXVECTOR4 bb1, bb2;

	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetMiddleTankAdr(0);
		for (j = 0; j < MIDDLETANK_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + MIDDLETANK_BB_LEFT, enemy->pos.y + MIDDLETANK_BB_TOP, MIDDLETANK_BB_WIDTH, MIDDLETANK_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	pBullet = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetMiddleTankAdr(0);
		for (j = 0; j < MIDDLETANK_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + MIDDLETANK_BB_LEFT, enemy->pos.y + MIDDLETANK_BB_TOP, MIDDLETANK_BB_WIDTH, MIDDLETANK_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	//ボムとの当たり判定
	PLAYERBOMBER *bom = GetPlayerBomberAdr(0);
	for (i = 0; i < PLAYERBOMBER_MAX; i++, bom++)
	{
		if (!bom->active)
		{
			continue;
		}
		enemy = GetMiddleTankAdr(0);
		for (j = 0; j < MIDDLETANK_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(bom->pos.x + PLAYERBOMBER_BB_LEFT, bom->pos.y + PLAYERBOMBER_BB_TOP, PLAYERBOMBER_BB_WIDTH * bom->scale, PLAYERBOMBER_BB_HIGHT * bom->scale);
			bb2 = D3DXVECTOR4(enemy->pos.x + MIDDLETANK_BB_LEFT, enemy->pos.y + MIDDLETANK_BB_TOP, MIDDLETANK_BB_WIDTH, MIDDLETANK_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= BOMBER_DAMAGE;
			}
		}
	}

	/* 戦車なのでプレイヤーとの当たり判定はオミット
	//プレイヤーとの当たり判定
	PLAYER *player = GetPlayerAdr(0);
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->state != PLAYER_DEFAULT)
		{
			continue;
		}

		enemy = GetMiddleTankAdr(0);
		for (j = 0; j < MIDDLETANK_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_BB_WIDTH, PLAYER_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + MIDDLETANK_BB_LEFT, enemy->pos.y + MIDDLETANK_BB_TOP, MIDDLETANK_BB_WIDTH, MIDDLETANK_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				DestroyPlayer(i);
			}
		}
	}
	*/
}

/******************************************************************************
//関数名	：void CollisionSmallTank(void)
//引数1		：void
//戻り値	：void
//説明		：スモールタンクの接触判定
******************************************************************************/
void CollisionSmallTank(void)
{
	int i, j;
	PLAYERBULLET *pBullet = GetPlayerBulletAdr(0);
	SMALLTANK *enemy = GetSmallTankAdr(0);
	D3DXVECTOR4 bb1, bb2;

	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetSmallTankAdr(0);
		for (j = 0; j < SMALLTANK_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLTANK_BB_LEFT, enemy->pos.y + SMALLTANK_BB_TOP, SMALLTANK_BB_WIDTH, SMALLTANK_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	pBullet = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetSmallTankAdr(0);
		for (j = 0; j < SMALLTANK_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLTANK_BB_LEFT, enemy->pos.y + SMALLTANK_BB_TOP, SMALLTANK_BB_WIDTH, SMALLTANK_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	//ボムとの当たり判定
	PLAYERBOMBER *bom = GetPlayerBomberAdr(0);
	for (i = 0; i < PLAYERBOMBER_MAX; i++, bom++)
	{
		if (!bom->active)
		{
			continue;
		}
		enemy = GetSmallTankAdr(0);
		for (j = 0; j < SMALLTANK_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(bom->pos.x + PLAYERBOMBER_BB_LEFT, bom->pos.y + PLAYERBOMBER_BB_TOP, PLAYERBOMBER_BB_WIDTH * bom->scale, PLAYERBOMBER_BB_HIGHT * bom->scale);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLTANK_BB_LEFT, enemy->pos.y + SMALLTANK_BB_TOP, SMALLTANK_BB_WIDTH, SMALLTANK_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= BOMBER_DAMAGE;
			}
		}
	}

	/* 戦車だしプレイヤーとの当たり判定はオミット
	//プレイヤーとの当たり判定
	PLAYER *player = GetPlayerAdr(0);
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (!player->active)
		{
			continue;
		}

		enemy = GetSmallTankAdr(0);
		for (j = 0; j < SMALLTANK_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_BB_WIDTH, PLAYER_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLTANK_BB_LEFT, enemy->pos.y + SMALLTANK_BB_TOP, SMALLTANK_BB_WIDTH, SMALLTANK_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				DestroyPlayer(i);
			}
		}
	}
	*/
}

/******************************************************************************
//関数名	：void CollisionSmallTank(void)
//引数1		：void
//戻り値	：void
//説明		：スモールザコ1の接触判定
******************************************************************************/
void CollisionSmallZako1(void)
{
	int i, j;
	PLAYERBULLET *pBullet = GetPlayerBulletAdr(0);
	SMALLZAKO1 *enemy = GetSmallZako1Adr(0);
	D3DXVECTOR4 bb1, bb2;

	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetSmallZako1Adr(0);
		for (j = 0; j < SMALLZAKO1_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLZAKO1_BB_LEFT, enemy->pos.y + SMALLZAKO1_BB_TOP, SMALLZAKO1_BB_WIDTH, SMALLZAKO1_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	pBullet = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetSmallZako1Adr(0);
		for (j = 0; j < SMALLZAKO1_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLZAKO1_BB_LEFT, enemy->pos.y + SMALLZAKO1_BB_TOP, SMALLZAKO1_BB_WIDTH, SMALLZAKO1_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	//ボムとの当たり判定
	PLAYERBOMBER *bom = GetPlayerBomberAdr(0);
	for (i = 0; i < PLAYERBOMBER_MAX; i++, bom++)
	{
		if (!bom->active)
		{
			continue;
		}
		enemy = GetSmallZako1Adr(0);
		for (j = 0; j < SMALLZAKO1_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(bom->pos.x + PLAYERBOMBER_BB_LEFT, bom->pos.y + PLAYERBOMBER_BB_TOP, PLAYERBOMBER_BB_WIDTH * bom->scale, PLAYERBOMBER_BB_HIGHT * bom->scale);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLZAKO1_BB_LEFT, enemy->pos.y + SMALLZAKO1_BB_TOP, SMALLZAKO1_BB_WIDTH, SMALLZAKO1_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= BOMBER_DAMAGE;
			}
		}
	}

	//プレイヤーとの当たり判定
	PLAYER *player = GetPlayerAdr(0);
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->state != PLAYER_DEFAULT)
		{
			continue;
		}

		enemy = GetSmallZako1Adr(0);
		for (j = 0; j < SMALLZAKO1_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_BB_WIDTH, PLAYER_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLZAKO1_BB_LEFT, enemy->pos.y + SMALLZAKO1_BB_TOP, SMALLZAKO1_BB_WIDTH, SMALLZAKO1_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				DestroyPlayer(i);
			}
		}
	}
}

/******************************************************************************
//関数名	：void CollisionSmallTank(void)
//引数1		：void
//戻り値	：void
//説明		：スモールザコ2の接触判定
******************************************************************************/
void CollisionSmallZako2(void)
{
	int i, j;
	PLAYERBULLET *pBullet = GetPlayerBulletAdr(0);
	SMALLZAKO2 *enemy = GetSmallZako2Adr(0);
	D3DXVECTOR4 bb1, bb2;

	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetSmallZako2Adr(0);
		for (j = 0; j < SMALLZAKO2_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLZAKO2_BB_LEFT, enemy->pos.y + SMALLZAKO2_BB_TOP, SMALLZAKO2_BB_WIDTH, SMALLZAKO2_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	pBullet = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetSmallZako2Adr(0);
		for (j = 0; j < SMALLZAKO2_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLZAKO2_BB_LEFT, enemy->pos.y + SMALLZAKO2_BB_TOP, SMALLZAKO2_BB_WIDTH, SMALLZAKO2_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	//ボムとの当たり判定
	PLAYERBOMBER *bom = GetPlayerBomberAdr(0);
	for (i = 0; i < PLAYERBOMBER_MAX; i++, bom++)
	{
		if (!bom->active)
		{
			continue;
		}
		enemy = GetSmallZako2Adr(0);
		for (j = 0; j < SMALLZAKO2_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(bom->pos.x + PLAYERBOMBER_BB_LEFT, bom->pos.y + PLAYERBOMBER_BB_TOP, PLAYERBOMBER_BB_WIDTH * bom->scale, PLAYERBOMBER_BB_HIGHT * bom->scale);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLZAKO2_BB_LEFT, enemy->pos.y + SMALLZAKO2_BB_TOP, SMALLZAKO2_BB_WIDTH, SMALLZAKO2_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= BOMBER_DAMAGE;
			}
		}
	}

	//プレイヤーとの当たり判定
	PLAYER *player = GetPlayerAdr(0);
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->state != PLAYER_DEFAULT)
		{
			continue;
		}

		enemy = GetSmallZako2Adr(0);
		for (j = 0; j < SMALLZAKO2_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_BB_WIDTH, PLAYER_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLZAKO2_BB_LEFT, enemy->pos.y + SMALLZAKO2_BB_TOP, SMALLZAKO2_BB_WIDTH, SMALLZAKO2_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				DestroyPlayer(i);
			}
		}
	}
}

/******************************************************************************
//関数名	：void CollisionItemCareer(void)
//引数1		：void
//戻り値	：void
//説明		：アイテムキャリアの接触判定
******************************************************************************/
void CollisionItemCareer(void)
{
	int i, j;
	PLAYERBULLET *pBullet = GetPlayerBulletAdr(0);
	ITEMCAREER *enemy = GetItemCareerAdr(0);
	D3DXVECTOR4 bb1, bb2;

	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetItemCareerAdr(0);
		for (j = 0; j < ITEMCAREER_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + ITEMCAREER_BB_LEFT, enemy->pos.y + ITEMCAREER_BB_TOP, ITEMCAREER_BB_WIDTH, ITEMCAREER_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	pBullet = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetItemCareerAdr(0);
		for (j = 0; j < ITEMCAREER_MAX; j++, enemy++)

		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + ITEMCAREER_BB_LEFT, enemy->pos.y + ITEMCAREER_BB_TOP, ITEMCAREER_BB_WIDTH, ITEMCAREER_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	//ボムとの当たり判定
	PLAYERBOMBER *bom = GetPlayerBomberAdr(0);
	for (i = 0; i < PLAYERBOMBER_MAX; i++, bom++)
	{
		if (!bom->active)
		{
			continue;
		}
		enemy = GetItemCareerAdr(0);
		for (j = 0; j < ITEMCAREER_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(bom->pos.x + PLAYERBOMBER_BB_LEFT, bom->pos.y + PLAYERBOMBER_BB_TOP, PLAYERBOMBER_BB_WIDTH * bom->scale, PLAYERBOMBER_BB_HIGHT * bom->scale);
			bb2 = D3DXVECTOR4(enemy->pos.x + ITEMCAREER_BB_LEFT, enemy->pos.y + ITEMCAREER_BB_TOP, ITEMCAREER_BB_WIDTH, ITEMCAREER_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= BOMBER_DAMAGE;
			}
		}
	}

	//プレイヤーとの当たり判定
	PLAYER *player = GetPlayerAdr(0);
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->state != PLAYER_DEFAULT)
		{
			continue;
		}

		enemy = GetItemCareerAdr(0);
		for (j = 0; j < ITEMCAREER_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}
			bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_BB_WIDTH, PLAYER_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + ITEMCAREER_BB_LEFT, enemy->pos.y + ITEMCAREER_BB_TOP, ITEMCAREER_BB_WIDTH, ITEMCAREER_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				DestroyPlayer(i);
			}
		}
	}

}

/******************************************************************************
//関数名	：void CollisionStage1BOss(void)
//引数1		：void
//戻り値	：void
//説明		：ステージ1ボスの接触判定
******************************************************************************/
void CollisionStage1Boss(void)
{
	int i;
	PLAYERBULLET *pBullet = GetPlayerBulletAdr(0);
	STAGE1BOSS *enemy = GetStage1BossAdr();
	D3DXVECTOR4 bb1, bb2;

	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetStage1BossAdr();

		if (!enemy->active)
		{
			continue;
		}

		bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
		bb2 = D3DXVECTOR4(enemy->pos.x + STAGE1BOSS_BB_LEFT, enemy->pos.y + STAGE1BOSS_BB_TOP, STAGE1BOSS_BB_WIDTH, STAGE1BOSS_BB_HIGHT);
		if (HitBB(bb1, bb2))
		{
			enemy->hp -= 1;
			SetPBulletExplosion(pBullet->pos);
			pBullet->active = false;
		}
	}

	pBullet = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetStage1BossAdr();
		if (!enemy->active)
		{
			break;;
		}

		bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
		bb2 = D3DXVECTOR4(enemy->pos.x + STAGE1BOSS_BB_LEFT, enemy->pos.y + STAGE1BOSS_BB_TOP, STAGE1BOSS_BB_WIDTH, STAGE1BOSS_BB_HIGHT);
		if (HitBB(bb1, bb2))
		{
			enemy->hp -= 1;
			SetPBulletExplosion(pBullet->pos);
			pBullet->active = false;
		}
	}

	//ボムとの当たり判定
	PLAYERBOMBER *bom = GetPlayerBomberAdr(0);
	for (i = 0; i < PLAYERBOMBER_MAX; i++, bom++)
	{
		if (!bom->active)
		{
			continue;
		}
		enemy = GetStage1BossAdr();
		if (!enemy->active)
		{
			continue;
		}

		bb1 = D3DXVECTOR4(bom->pos.x + PLAYERBOMBER_BB_LEFT, bom->pos.y + PLAYERBOMBER_BB_TOP, PLAYERBOMBER_BB_WIDTH * bom->scale, PLAYERBOMBER_BB_HIGHT * bom->scale);
		bb2 = D3DXVECTOR4(enemy->pos.x + STAGE1BOSS_BB_LEFT, enemy->pos.y + STAGE1BOSS_BB_TOP, STAGE1BOSS_BB_WIDTH, STAGE1BOSS_BB_HIGHT);
		if (HitBB(bb1, bb2))
		{
			enemy->hp -= BOMBER_DAMAGE;
		}

	}

	//プレイヤーとの当たり判定
	PLAYER *player = GetPlayerAdr(0);
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->state != PLAYER_DEFAULT)
		{
			continue;
		}

		enemy = GetStage1BossAdr();
		if (!enemy->active)
		{
			break;
		}

		bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_BB_WIDTH, PLAYER_BB_HIGHT);
		bb2 = D3DXVECTOR4(enemy->pos.x + STAGE1BOSS_BB_LEFT, enemy->pos.y + STAGE1BOSS_BB_TOP, STAGE1BOSS_BB_WIDTH, STAGE1BOSS_BB_HIGHT);
		if (HitBB(bb1, bb2))
		{
			DestroyPlayer(i);
		}

	}
}

/******************************************************************************
//関数名	：void CollisionTutorialEnemy(void)
//引数1		：void
//戻り値	：void
//説明		：ステージ1ボスの接触判定
******************************************************************************/
void CollisionTutorialEnemy(void)
{
	int i, j;
	PLAYERBULLET *pBullet = GetPlayerBulletAdr(0);
	TUTORIALENEMY *enemy = GetTutorialEnemyAdr(0);
	D3DXVECTOR4 bb1, bb2;


	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetTutorialEnemyAdr(0);
		for (j = 0; j < TUTORIALENEMY_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + TUTORIALENEMY_BB_LEFT, enemy->pos.y + TUTORIALENEMY_BB_TOP, TUTORIALENEMY_BB_WIDTH, TUTORIALENEMY_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	pBullet = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetTutorialEnemyAdr(0);
		for (j = 0; j < TUTORIALENEMY_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + TUTORIALENEMY_BB_LEFT, enemy->pos.y + TUTORIALENEMY_BB_TOP, TUTORIALENEMY_BB_WIDTH, TUTORIALENEMY_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}


	//ボムとの当たり判定
	PLAYERBOMBER *bom = GetPlayerBomberAdr(0);
	for (i = 0; i < PLAYERBOMBER_MAX; i++, bom++)
	{
		if (!bom->active)
		{
			continue;
		}

		enemy = GetTutorialEnemyAdr(0);
		for (j = 0; j < TUTORIALENEMY_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(bom->pos.x + PLAYERBOMBER_BB_LEFT, bom->pos.y + PLAYERBOMBER_BB_TOP, PLAYERBOMBER_BB_WIDTH * bom->scale, PLAYERBOMBER_BB_HIGHT * bom->scale);
			bb2 = D3DXVECTOR4(enemy->pos.x + TUTORIALENEMY_BB_LEFT, enemy->pos.y + TUTORIALENEMY_BB_TOP, TUTORIALENEMY_BB_WIDTH, TUTORIALENEMY_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= BOMBER_DAMAGE;
			}
		}
	}

	//プレイヤーとの当たり判定
	PLAYER *player = GetPlayerAdr(0);
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->state != PLAYER_DEFAULT)
		{
			continue;
		}

		enemy = GetTutorialEnemyAdr(0);
		for (j = 0; j < TUTORIALENEMY_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_BB_WIDTH, PLAYER_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + TUTORIALENEMY_BB_LEFT, enemy->pos.y + TUTORIALENEMY_BB_TOP, TUTORIALENEMY_BB_WIDTH, TUTORIALENEMY_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				DestroyPlayer(i);
			}
		}
	}
}

/******************************************************************************
//関数名	：void CollisionPlayerShield(void)
//引数1		：void
//戻り値	：void
//説明		：プレイヤーシールドの接触判定
******************************************************************************/
/*
void CollisionPlayerShield(void)
{
	PLAYERSHIELD *shield = GetPlayerShieldAdr();
	ENEMYBULLET *bullet = GetEnemyBulletAdr(GREEN_ROTATE, 0);

	if (!shield->active)
	{
		return;
	}

	for (int i = 0; i < ENEMYBULLET_MAX / 2; i++, bullet++)
	{
		if (!bullet->active)
		{
			continue;
		}

		if (HitBC(shield->pos, bullet->pos, PLAYERSHIELD_BC_RADIUS, ENEMYBULLET_BC_RADIUS))
		{
			bullet->active = false;
		}
	}
}
*/

/******************************************************************************
//関数名	：void CollisionBomber(void)
//引数1		：void
//戻り値	：void
//説明		：プレイヤーボンバーの接触判定
******************************************************************************/
void CollisionBomber(void)
{
	PLAYERBOMBER *bomber = GetPlayerBomberAdr(0);
	ENEMYBULLET *bullet = GetEnemyBulletAdr(GREEN_ROTATE, 0);

	for (int i = 0; i < PLAYERBOMBER_MAX; i++, bomber++)
	{
		if (!bomber->active)
		{
			continue;
		}

		bullet = GetEnemyBulletAdr(GREEN_ROTATE, 0);
		for (int j = 0; j < ENEMYBULLET_MAX; j++, bullet++)
		{
			if (HitBC(bomber->pos, bullet->pos, PLAYERBOMBER_BC_RADIUS * bomber->scale, ENEMYBULLET_BC_RADIUS))
			{
				bullet->active = false;
			}
		}
	}

}

/******************************************************************************
//関数名	：void CollisionPlayerShield(void)
//引数1		：void
//戻り値	：void
//説明		：プレイヤーシールドの接触判定
******************************************************************************/
void CollisionPlayerShield(void)
{
	PLAYERSHIELD *shield = GetPlayerShieldAdr();
	ENEMYBULLET *bullet = GetEnemyBulletAdr(GREEN_ROTATE, 0);

	if (!shield->active)
	{
		return;
	}

	for (int i = 0; i < ENEMYBULLET_MAX; i++, bullet++)
	{
		if (!bullet->active)
		{
			continue;
		}

		if (HitBC(shield->pos, bullet->pos, PLAYERSHIELD_BC_RADIUS, ENEMYBULLET_BC_RADIUS))
		{
			bullet->active = false;
			SetEnemyBulletExplosion(bullet->pos, bullet->type);
		}
	}
}