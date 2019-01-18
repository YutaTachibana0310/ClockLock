//=============================================================================
//
// プレイヤー処理 [Player.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "playerBullet.h"
#include "bulletFire.h"
#include "bg.h"
#include "input.h"
#include "enemyBullet.h"
#include "playerGauge.h"
#include "playerExplosion.h"
#include "transition.h"
#include "gameOver.h"
#include "gameClear.h"
#include "playerShield.h"
#include "playerBomber.h"
#include "GUI.h"
#include "sePlayer.h"
#include "playerParticle.h"
#include "playerShield.h"

/******************************************************************************
マクロ定義
*******************************************************************************/
#define PLAYER_SPEED			(6.0f)									//プレイヤーの移動速度					
#define PLAYER_SPEED_SLOW		(3.0f)

#define PLAYER_SHOTPOS_L		(D3DXVECTOR3(-20.0f, -50.0f, 0.0f))		//プレイヤーバレットの発射位置（左）
#define PLAYER_SHOTPOS_R		(D3DXVECTOR3(20.0f, -50.0f, 0.0f))		//プレイヤーバレットの発射位置（右）
#define PLAYER_SIDESHOTPOS_L	(D3DXVECTOR3(-40.0f, -20.0f, 0.0f))		//プレイヤーバレットの横発射位置（左）
#define PLAYER_SIDESHOTPOS_R	(D3DXVECTOR3(40.0f, -20.0f, 0.0f))		//プレイヤーバレットの横発射位置（右）

#define PLAYER_FIREPOS_L		(D3DXVECTOR3(-15.0f, -50.0f, 0.0f))		//バレットファイアのセット位置（左）
#define PLAYER_FIREPOS_R		(D3DXVECTOR3(15.0f, -50.0f, 0.0f))		//バレットファイアのセット位置（右）
#define PLAYER_SIDEFIREPOS_L	(D3DXVECTOR3(-35.0f, -20.0f, 0.0f))		//バレットファイアのセット位置（サブ左）
#define PLAYER_SIDEFIREPOS_R	(D3DXVECTOR3(35.0f, -20.0f, 0.0f))		//バレットファイアのセット位置（サブ右）

#define PLAYER_SIZE_X			(PLAYER_TEXTURE_SIZE_X)
#define PLAYER_SIZE_Y			(PLAYER_TEXTURE_SIZE_Y-20)

#define PLAYER_SHOTINTERBAL		(1)										//プレイヤーバレットの発射間隔
#define PLAYER_SHOTANGLE		(2.5f)									//プレイヤーバレットの発射角度（サブショット用）
#define PLAYER_SHOTNUM_MAX		(6)										//shotNumの最大値

#define PLAYER_TIMEEFFECT_DECREASEEASY		(1.0f)						//時間操作中に1フレーム毎にゲージが減少する値(EASY)
#define PLAYER_TIMEEFFECT_DECREASENORMAL	(1.2f)						//時間操作中に1フレーム毎にゲージが減少する値(NORMAL)
#define PLAYER_TIMEEFFECT_DECREASEHARD		(1.5f)						//時間操作中に1フレーム毎にゲージが減少する値(HARD)

#define PLAYER_SHADOW_POS		(D3DXVECTOR3(30.0f, 100.0f, 0.0f))		//プレイヤーの影の座標
#define PLAYER_SHADOW_SCALE		(0.5f)									//プレイヤーの影のスケール

#define PLAYER_INITIALIZED_LENGTH	(20.0f)								//INITIALIZED状態で移動する距離
#define PLAYER_INITIALIZED_FRAME	(30)								//INITIALIZED状態から遷移する時間

#define PLAYER_INITZANKI			(2)									//プレイヤーの初期残機
#define PLAYER_INITZANKI_DEMO		(89)								//デモ用の初期残機

#define PLAYER_BOMBER_OFFSETPOS		(D3DXVECTOR3(0.0f, -500.0f, 0.0f))	//ボンバー投下位置
#define PLAYER_BOMBER_INITCNT		(3)									//初期ボンバー数

#define PLAYER_TIMEEFFECT_SEPERIOD	(30)								//時間操作中に効果音を鳴らす周期

#define PLAYER_PARTICLE_EMITNUM		(100)
#define PLAYER_PARTICLE_MAXSCALE	(5.0f)
#define PLAYER_PARTICLE_DURATION	(15)

#define PLAYER_INVINCIBLE_FRAME		(300)

#define PLAYER_TRAIL_SETFRAME		(10)
/******************************************************************************
構造体定義
*******************************************************************************/
enum PLAYER_DIR
{
	PLAYER_LEFT,
	PLAYER_FORWARD,
	PLAYER_RIGHT
};

enum PLAYER_TEXTUREINDEX
{
	PLAYER_BODY,
	PLAYER_COLLIDER,
	PLAYER_TEXMAX
};

/******************************************************************************
プロトタイプ宣言
*******************************************************************************/
void UpdateInitializedPlayer(PLAYER *ptr);					//初期状態のプレイヤー更新処理
void UpdateDefaultPlayer(PLAYER *ptr, int i);				//通常状態のプレイヤー更新処理
void UpdateDestroyedPlayer(PLAYER *ptr);					//破壊状態のプレイヤー更新処理
void UpdateInvinciblePlayer(PLAYER *ptr);					//無敵状態のプレイヤー更新処理

HRESULT MakeVertexPlayer(void);								//頂点の作成
void SetTexturePlayer(int cntPattern, PLAYER *ptr);			//テクスチャの計算処理 
void SetTexturePlayerCore(PLAYER *ptr);						//テクスチャの計算処理（コア用）
void SetVertexPlayer(PLAYER *ptr);							//頂点の計算処理
void SetVertexPlayerCore(PLAYER *ptr);						//頂点の計算処理（コア用）
void MovePlayer(PLAYER* ptr, int i);						//プレイヤーの移動処理
void PlayerAttack(PLAYER *ptr);								//プレイヤーの攻撃処理
void ProcessTimeEffect(PLAYER *ptr, int i);					//時間操作処理
void PlayerAnimation(PLAYER *ptr);							//アニメーション処理
void ProcessBulletFire(PLAYER *ptr);						//バレットファイア処理

void SetPlayerTrail(PLAYER *ptr);							//トレイル更新処理
void SetVertexPlayerTrail(PLAYER *ptr, int trailIndex);		//トレイル頂点設定処理

/******************************************************************************
グローバル変数
*******************************************************************************/
static LPDIRECT3DTEXTURE9		texture[PLAYER_TEXMAX];		// テクスチャへのポインタ

PLAYER					player[PLAYER_MAX];					// プレイヤー構造体

D3DXVECTOR3				rotPlayer;							// ポリゴンの回転量

float					fRadiusPlayer;						// ポリゴンの半径
float					fBaseAnglePlayer;					// ポリゴンの角度
int						playerDir;							// プレイヤーの左右の移動方向

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitPlayer(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i, j;
	PLAYER *ptr = GetPlayerAdr(0);

	for (i = 0; i < PLAYER_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(PLAYER_INITPOS_X, PLAYER_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(PLAYER_TEXTURE_SIZE_Y / 2.0f, PLAYER_TEXTURE_SIZE_X / 2.0f);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(PLAYER_TEXTURE_SIZE_Y / 2.0f, PLAYER_TEXTURE_SIZE_X / 2.0f));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->zanki = (isDemoMode()) ? PLAYER_INITZANKI_DEMO : PLAYER_INITZANKI;

		ptr->shotInterbal = 0;
		ptr->numFireShot = 0;
		ptr->playerDir = PLAYER_FORWARD;

		ptr->angleCore = atan2f(PLAYER_CORE_SIZE_Y, PLAYER_CORE_SIZE_X);
		ptr->radiusCore = D3DXVec2Length(&D3DXVECTOR2(PLAYER_CORE_SIZE_X, PLAYER_CORE_SIZE_Y));

		ptr->timeEffect = PLAYER_TIMEEFFECT_MAX;
		ptr->cntFrame = 0;

		int difficulty = GetDifficulty();
		ptr->gaugeDeltaValue = (difficulty == DIFFICULTY_EASY) ? PLAYER_TIMEEFFECT_DECREASEEASY : (difficulty == DIFFICULTY_NORMAL) ? PLAYER_TIMEEFFECT_DECREASENORMAL : PLAYER_TIMEEFFECT_DECREASEHARD;

		for (j = 0; j < PLAYER_NUM_BULLETFIRE; j++)
		{
			ptr->fire[j] = GetBulletFireAdr(j + i * PLAYER_NUM_BULLETFIRE);
			ptr->fire[j]->active = false;
		}

		ptr->state = PLAYER_INITIALIZED;
		ptr->bomCnt = PLAYER_BOMBER_INITCNT;
		ptr->corePattern = 0;

		//各機能のロック（チュートリアル用）
		if (num == 2)
		{
			ptr->lockBomber = true;
			ptr->lockTimeEffect = true;
		}
		else
		{
			ptr->lockBomber = false;
			ptr->lockTimeEffect = false;
		}

		ptr->active = true;
	}

	// 頂点情報の作成
	MakeVertexPlayer();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture[PLAYER_COLLIDER] = CreateTextureFromFile((LPSTR)PLAYER_CORETEX_NAME, pDevice);
		texture[PLAYER_BODY] = CreateTextureFromFile((LPSTR)PLAYER_TEXTURE, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitPlayer(void)
{
	for (int i = 0; i < PLAYER_TEXMAX; i++)
	{
		if (texture[i] != NULL)
		{	// テクスチャの開放
			texture[i]->Release();
			texture[i] = NULL;
		}
	}
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdatePlayer(void)
{
	PLAYER *ptr = GetPlayerAdr(0);

	int i;

	for (i = 0; i < PLAYER_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		ptr->countAnim++;

		switch (ptr->state)
		{
		case PLAYER_INITIALIZED:
			UpdateInitializedPlayer(ptr);
			break;

		case PLAYER_INVINCIBLE:
			UpdateInvinciblePlayer(ptr);

		case PLAYER_DEFAULT:
			UpdateDefaultPlayer(ptr, i);
			break;

		case PLAYER_DESTROYED:
			UpdateDestroyedPlayer(ptr);
			break;
		}
	}
}

/******************************************************************************
更新処理(初期状態)
******************************************************************************/
void UpdateInitializedPlayer(PLAYER *ptr)
{
	float rad = RADIAN((float)ptr->countAnim / PLAYER_INITIALIZED_FRAME * 90);
	ptr->pos.y -= PLAYER_INITIALIZED_LENGTH * cosf(rad);

	if (player->countAnim >= PLAYER_INITIALIZED_FRAME)
	{
		player->countAnim = 0;
		player->state = PLAYER_INVINCIBLE;
	}
}

/******************************************************************************
更新処理（無敵状態）
******************************************************************************/
void UpdateInvinciblePlayer(PLAYER *ptr)
{
	if (ptr->countAnim >= PLAYER_INVINCIBLE_FRAME)
	{
		player->state = PLAYER_DEFAULT;
	}
}

/******************************************************************************
更新処理（通常状態）
******************************************************************************/
void UpdateDefaultPlayer(PLAYER *ptr, int i)
{
	ptr->cntFrame++;

	//トレイル更新処理
	SetPlayerTrail(ptr);

	//移動処理
	MovePlayer(ptr, i);

	// アニメーション
	PlayerAnimation(ptr);

	//バレットファイアの位置更新
	ProcessBulletFire(ptr);

	//攻撃処理
	ptr->shotInterbal++;
	if (GetMyButton0Down(i) && ptr->shotInterbal > PLAYER_SHOTINTERBAL)
	{
		PlayerAttack(ptr);
		ptr->shotInterbal = 0;
		ptr->numFireShot = (ptr->numFireShot + 1) % PLAYER_SHOTNUM_MAX;
	}

	//ボンバー発射処理
	if (GetMyButton2Trigger(0) && ptr->bomCnt > 0 && !ptr->lockBomber)
	{
		//ボンバー発射に成功したら
		if (SetPlayerBomber(ptr->pos + PLAYER_BOMBER_OFFSETPOS))
		{
			PlaySE(SE_BOMBER);
			ptr->bomCnt--;
			DisableAllEnemyBullet();
			SetGUIShake();
		}
	}

	//時間操作処理
	ProcessTimeEffect(ptr, i);
	
}

/******************************************************************************
更新処理（破壊状態）
******************************************************************************/
void UpdateDestroyedPlayer(PLAYER *ptr)
{
	if (ptr->countAnim >= 60)
	{
		RespornPlayer(0);
	}
}

/********************************************************************************
描画処理
*******************************************************************************/
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *ptr = GetPlayerAdr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (i = 0; i < PLAYER_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		// テクスチャの設定
		pDevice->SetTexture(0, texture[PLAYER_BODY]);
		SetTexturePlayer(ptr->patternAnim, ptr);

		//トレイルの描画
		//if (GetSpeedType() == SLOW_GREEN)
		//{
		//	for (int j = 1; j < PLAYER_TRAILPOS_NUM; j*= 10)
		//	{
		//		SetVertexPlayerTrail(ptr, j);
		//		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, ptr->vertexWk, sizeof(VERTEX_2D));
		//	}
		//}

		// 頂点の計算処理
		SetVertexPlayer(ptr);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, ptr->shadowWk, sizeof(VERTEX_2D));
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, ptr->vertexWk, sizeof(VERTEX_2D));

		//当たり判定表示用の描画
		pDevice->SetTexture(0, texture[PLAYER_COLLIDER]);
		SetVertexPlayerCore(ptr);
		SetTexturePlayerCore(ptr);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, ptr->vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
		//当たり判定の描画
		//DrawBoxCollider(ptr->pos + D3DXVECTOR3(PLAYER_BB_LEFT, PLAYER_BB_TOP, 0.0f), PLAYER_BB_WIDTH, PLAYER_BB_HIGHT);
		//DrawBoxCollider(ptr->pos + D3DXVECTOR3(PLAYER_BB_LEFT, PLAYER_BB_TOP, 0.0f), PLAYER_ITEMBB_WIDTH, PLAYER_ITEMBB_HIGHT);
#endif
	}
}

/******************************************************************************
頂点の作成
*******************************************************************************/
HRESULT MakeVertexPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *ptr = GetPlayerAdr(0);
	int i;

	for (i = 0; i < PLAYER_MAX; i++, ptr++)
	{
		// 頂点座標の設定	
		SetVertexPlayer(ptr);

		// rhwの設定
		ptr->vertexWk[0].rhw =
			ptr->vertexWk[1].rhw =
			ptr->vertexWk[2].rhw =
			ptr->vertexWk[3].rhw = 1.0f;

		ptr->shadowWk[0].rhw =
			ptr->shadowWk[1].rhw =
			ptr->shadowWk[2].rhw =
			ptr->shadowWk[3].rhw = 1.0f;

		// 反射光の設定
		ptr->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		ptr->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		ptr->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		ptr->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		ptr->shadowWk[0].diffuse = D3DCOLOR_RGBA(0, 0, 0, 128);
		ptr->shadowWk[1].diffuse = D3DCOLOR_RGBA(0, 0, 0, 128);
		ptr->shadowWk[2].diffuse = D3DCOLOR_RGBA(0, 0, 0, 128);
		ptr->shadowWk[3].diffuse = D3DCOLOR_RGBA(0, 0, 0, 128);

		// テクスチャ座標の設定
		SetTexturePlayer(ptr->patternAnim, ptr);

	}

	return S_OK;
}

/******************************************************************************
テクスチャ座標の設定
******************************************************************************/
void SetTexturePlayer(int cntPattern, PLAYER *ptr)
{
	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X;
	int y = ptr->playerDir;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;

	ptr->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	ptr->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	ptr->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	ptr->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

	ptr->shadowWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	ptr->shadowWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	ptr->shadowWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	ptr->shadowWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

/******************************************************************************
テクスチャ座標の設定(コア用)
******************************************************************************/
void SetTexturePlayerCore(PLAYER *ptr)
{
	int x = ptr->corePattern;
	int y = 0;

	float sizeX = 1.0f / PLAYER_CORE_DIVIDE_X;
	float sizeY = 1.0f / PLAYER_CORE_DIVIDE_Y;

	ptr->vertexWk[0].tex = D3DXVECTOR2(x * sizeX, 0.0f);
	ptr->vertexWk[1].tex = D3DXVECTOR2(x * sizeX + sizeX, 0.0f);
	ptr->vertexWk[2].tex = D3DXVECTOR2(x * sizeX, 1.0f);
	ptr->vertexWk[3].tex = D3DXVECTOR2(x * sizeX + sizeX, 1.0f);
}

/*******************************************************************************
頂点座標の設定
*******************************************************************************/
void SetVertexPlayer(PLAYER *ptr)
{
	// 頂点座標の設定
	ptr->vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	ptr->vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius;
	ptr->vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	ptr->vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	ptr->vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	ptr->vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	ptr->vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	ptr->vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius;

	ptr->shadowWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius * PLAYER_SHADOW_SCALE + PLAYER_SHADOW_POS.x;
	ptr->shadowWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius* PLAYER_SHADOW_SCALE + PLAYER_SHADOW_POS.y;
	ptr->shadowWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius* PLAYER_SHADOW_SCALE + PLAYER_SHADOW_POS.x;
	ptr->shadowWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius* PLAYER_SHADOW_SCALE + PLAYER_SHADOW_POS.y;
	ptr->shadowWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius* PLAYER_SHADOW_SCALE + PLAYER_SHADOW_POS.x;
	ptr->shadowWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius* PLAYER_SHADOW_SCALE + PLAYER_SHADOW_POS.y;
	ptr->shadowWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius* PLAYER_SHADOW_SCALE + PLAYER_SHADOW_POS.x;
	ptr->shadowWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius* PLAYER_SHADOW_SCALE + PLAYER_SHADOW_POS.y;

	//ディフューズの設定
	ptr->vertexWk[0].diffuse =
		ptr->vertexWk[1].diffuse =
		ptr->vertexWk[2].diffuse =
		ptr->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
}

/*******************************************************************************
頂点座標の設定(コア用)
*******************************************************************************/
void SetVertexPlayerCore(PLAYER *ptr)
{
	D3DXVECTOR3 pos = ptr->pos + D3DXVECTOR3(0.0f, PLAYER_CORE_OFFSET_Y, 0.0f);
	//頂点座標の設定
	ptr->vertexWk[0].vtx.x = pos.x - cosf(ptr->angleCore) * ptr->radiusCore;
	ptr->vertexWk[0].vtx.y = pos.y - sinf(ptr->angleCore) * ptr->radiusCore;
	ptr->vertexWk[1].vtx.x = pos.x + cosf(ptr->angleCore) * ptr->radiusCore;
	ptr->vertexWk[1].vtx.y = pos.y - sinf(ptr->angleCore) * ptr->radiusCore;
	ptr->vertexWk[2].vtx.x = pos.x - cosf(ptr->angleCore) * ptr->radiusCore;
	ptr->vertexWk[2].vtx.y = pos.y + sinf(ptr->angleCore) * ptr->radiusCore;
	ptr->vertexWk[3].vtx.x = pos.x + cosf(ptr->angleCore) * ptr->radiusCore;
	ptr->vertexWk[3].vtx.y = pos.y + sinf(ptr->angleCore) * ptr->radiusCore;
}

/******************************************************************************
//関数名	：PLAYER *GetPlayerAdr(int pno)
//引数		：int pno：取得したいプレイヤーのインデックス
//戻り値	：プレイヤーのアドレス
//説明		：プレイヤーのアドレス取得関数
******************************************************************************/
PLAYER *GetPlayerAdr(int pno)
{
	if (pno < PLAYER_MAX)
	{
		return &player[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//関数名	：void MovePlayer(D3DXVECTOR3 *dir, PLAYER *ptr)
//引数1		：D3DXVECTOR3 *dir：移動方向へのベクトル
//引数2		：PLAYER *ptr：移動させるプレイヤーへのポインタ
//戻り値	：void
//説明		：プレイヤーの移動処理
******************************************************************************/
void MovePlayer(PLAYER *ptr, int i)
{

	D3DXVECTOR3 dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動方向を格納するベクトル

	dir.y = (float)GetMyAxisY(i);
	dir.x = (float)GetMyAxisX(i);
	D3DXVec3Normalize(&dir, &dir);

	ptr->playerDir = GetMyAxisX(i) + 1;

	//ベクトルの各成分に移動速度をかけて座標を更新
	float speed = (GetMySlowButtonDown(i)) ? PLAYER_SPEED_SLOW : PLAYER_SPEED;
	ptr->pos.x += dir.x * speed;
	ptr->pos.y += dir.y * speed;


	//X軸方向にはみ出ているかの判定
	if (ptr->pos.x > SCREEN_END_RIGHT - PLAYER_SIZE_X / 2.0f)
	{
		ptr->pos.x = SCREEN_END_RIGHT - PLAYER_SIZE_X / 2.0f;
	}
	else if (ptr->pos.x < SCREEN_END_LEFT + PLAYER_SIZE_X / 2.0f)
	{
		ptr->pos.x = SCREEN_END_LEFT + PLAYER_SIZE_X / 2.0f;
	}

	//Y軸方向にはみ出ているかの判定
	if (ptr->pos.y > SCREEN_HEIGHT - PLAYER_SIZE_Y / 2.0f + 20.0f)
	{
		ptr->pos.y = SCREEN_HEIGHT - PLAYER_SIZE_Y / 2.0f + 20.0f;
	}
	else if (ptr->pos.y < PLAYER_SIZE_Y / 2.0f)
	{
		ptr->pos.y = PLAYER_SIZE_Y / 2.0f;
	}
}

/******************************************************************************
//関数名	：void PlayerAttack(PLAYER *ptr)
//引数1		：PLAYER *ptr：攻撃するプレイヤーへのポインタ
//戻り値	：void
//説明		：プレイヤーの攻撃処理
******************************************************************************/
void PlayerAttack(PLAYER *ptr)
{
	int i;

	//効果音再生
	PlaySE(SE_PLAYERSHOT);


	//SHOTPOSからプレイヤーバレットを発射
	FirePlayerBullet(ptr->pos + PLAYER_SHOTPOS_R);
	FirePlayerBullet(ptr->pos + PLAYER_SHOTPOS_L);

	SetBulletFire(ptr->fire[0]);
	SetBulletFire(ptr->fire[1]);

	//SIDESHOTPOSからサブバレットを発射
	for (i = ptr->numFireShot; i < ptr->numFireShot + 3; i++)
	{
		FirePlayerSubBullet(ptr->pos + PLAYER_SIDESHOTPOS_L, 90 - (i + 1) * PLAYER_SHOTANGLE);
		FirePlayerSubBullet(ptr->pos + PLAYER_SIDESHOTPOS_R, 90 + (i + 1) * PLAYER_SHOTANGLE);
	}

	SetBulletFire(ptr->fire[2]);
	SetBulletFire(ptr->fire[3]);
}

/******************************************************************************
//関数名	：void ProcessTimeEffect(PLAYER *ptr)
//引数1		：PLAYER *ptr：処理するプレイヤーへのポインタ
//戻り値	：void
//説明		：時間操作処理
******************************************************************************/
void ProcessTimeEffect(PLAYER *ptr, int i)
{
	//時間操作機能がロック中であればリターン
	if (ptr->lockTimeEffect)
	{
		return;
	}

	//効果音再生
	//if (ptr->cntFrame % PLAYER_TIMEEFFECT_SEPERIOD == 0)
	//{
	//	//PlaySE(SE_CLOCK);
	//}

	PLAYERGAUGE *gauge = GetPlayerGaugeAdr(i);

	if (ptr->timeEffect > 0.0f && GetMyButton1Down(i))
	{

		gauge->state = GAUGESTATE_OPEN;
		ptr->timeEffect = Clampf(0.0f, PLAYER_TIMEEFFECT_MAX, ptr->timeEffect - ptr->gaugeDeltaValue);
		SetSpeedType(SLOW_GREEN);
		gauge->percent = ptr->timeEffect;
		return;
	}

	SetSpeedType(DEFAULT_SPEED);
	gauge->state = GAUGESTATE_CLOSE;
}

/******************************************************************************
//関数名	：void PlayerAnimation(PLAYER *ptr)
//引数1		：PLAYER *ptr：処理するプレイヤーへのポインタ
//戻り値	：void
//説明		：アニメーション処理
******************************************************************************/
void PlayerAnimation(PLAYER *ptr)
{
	// アニメーションWaitチェック
	if ((ptr->countAnim % TIME_ANIMATION) == 0)
	{
		// パターンの切り替え
		ptr->patternAnim = (ptr->patternAnim + 1) % ANIM_PATTERN_NUM;
		ptr->corePattern = (ptr->corePattern + 1) % PLAYER_CORE_PATTERNMAX;
	}
}

/******************************************************************************
//関数名	：void ProcessBulletFire(PLAYER *ptr)
//引数1		：PLAYER *ptr：処理するプレイヤーへのポインタ
//戻り値	：void
//説明		：バレットファイア処理
******************************************************************************/
void ProcessBulletFire(PLAYER *ptr)
{
	ptr->fire[0]->pos = ptr->pos + PLAYER_FIREPOS_R;
	ptr->fire[1]->pos = ptr->pos + PLAYER_FIREPOS_L;
	ptr->fire[2]->pos = ptr->pos + PLAYER_SIDEFIREPOS_L;
	ptr->fire[3]->pos = ptr->pos + PLAYER_SIDEFIREPOS_R;
}

/******************************************************************************
//関数名	：void DestroyPlayer(PLAYER *ptr)
//引数1		：PLAYER *ptr：処理するプレイヤーへのポインタ
//戻り値	：void
//説明		：撃墜処理
******************************************************************************/
void DestroyPlayer(int i)
{
	PLAYER *player = GetPlayerAdr(i);

	SetPlayerExplosion(player->pos);
	SetEmitterPlayerParticle(player->pos, PLAYER_PARTICLE_DURATION, PLAYER_PARTICLE_EMITNUM, EMITTER_CIRCLE, PLAYER_PARTICLE_MAXSCALE);

	player->state = PLAYER_DESTROYED;
	player->pos = D3DXVECTOR3(PLAYER_INITPOS_X, PLAYER_INITPOS_Y, 0.0f);
	player->countAnim = 0;

	PLAYERGAUGE *gauge = GetPlayerGaugeAdr(0);
	gauge->scale = 0.0f;
	gauge->state = GAUGESTATE_NORMAL;
	SetSpeedType(DEFAULT_SPEED);
}

/******************************************************************************
//関数名	：void RespornPlayer(PLAYER *ptr)
//引数1		：PLAYER *ptr：処理するプレイヤーへのポインタ
//戻り値	：void
//説明		：リスポーン処理
******************************************************************************/
void RespornPlayer(int i)
{
	PLAYER *ptr = GetPlayerAdr(i);

	//残機が残っている場合
	if (ptr->zanki > 0)
	{
		//チュートリアルシーンでは残機を減らさない
		if (GetCurrentScene() != SCENE_TUTORIAL)
		{
			ptr->zanki -= 1;
		}

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->shotInterbal = 0;
		ptr->numFireShot = 0;
		ptr->playerDir = 1;
		ptr->cntFrame = 0;

		ptr->timeEffect = PLAYER_TIMEEFFECT_MAX;

		for (int j = 0; j < PLAYER_NUM_BULLETFIRE; j++)
		{
			ptr->fire[j]->active = false;
		}

		ptr->bomCnt = PLAYER_BOMBER_INITCNT;
		SetPlayerShield();
		ptr->state = PLAYER_INITIALIZED;
	}
	//残機ゼロの場合
	else
	{
		SetGameOver();
		ptr->state = PLAYER_GAMEOVER;
	}
}

/******************************************************************
//関数名	：SetPlayerTrail(PLAYER *ptr)
//引数		：PLAYER *ptr：トレイルを保存するプレイヤーへのポインタ
//戻り値	：void
//説明		：プレイヤーのトレイルを更新する処理
*******************************************************************/
void SetPlayerTrail(PLAYER *ptr)
{
	//トレイル座標配列をシフト
	VectorArrayShiftDown(&ptr->trailPos[0], 0, PLAYER_TRAILPOS_NUM);

	//現在の座標をトレイル配列に保存
  	ptr->trailPos[0] = ptr->pos;
}

/******************************************************************
//関数名	：SetVertexPlayerTrail(PLAYER *ptr, int trailIndex)
//引数1		：PLAYER *ptr：トレイルを描画するプレイヤーへのポインタ
//引数2		：int trailIndex：描画するトレイルのインデックス
//戻り値	：void
//説明		：プレイヤーのトレイルの頂点を設定する処理
*******************************************************************/
void SetVertexPlayerTrail(PLAYER *ptr, int trailIndex)
{
	//頂点座標の設定
	ptr->vertexWk[0].vtx.x = ptr->trailPos[trailIndex].x - cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	ptr->vertexWk[0].vtx.y = ptr->trailPos[trailIndex].y - sinf(ptr->angle + ptr->rot.z) * ptr->radius;
	ptr->vertexWk[1].vtx.x = ptr->trailPos[trailIndex].x + cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	ptr->vertexWk[1].vtx.y = ptr->trailPos[trailIndex].y - sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	ptr->vertexWk[2].vtx.x = ptr->trailPos[trailIndex].x - cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	ptr->vertexWk[2].vtx.y = ptr->trailPos[trailIndex].y + sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	ptr->vertexWk[3].vtx.x = ptr->trailPos[trailIndex].x + cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	ptr->vertexWk[3].vtx.y = ptr->trailPos[trailIndex].y + sinf(ptr->angle + ptr->rot.z) * ptr->radius;

	//ディフューズの設定
	int alpha = (int)(255 * ((float)PLAYER_TRAILPOS_NUM - trailIndex) / PLAYER_TRAILPOS_NUM);
	ptr->vertexWk[0].diffuse =
		ptr->vertexWk[1].diffuse =
		ptr->vertexWk[2].diffuse =
		ptr->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
}