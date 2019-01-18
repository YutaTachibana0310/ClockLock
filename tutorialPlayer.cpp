//=============================================================================
//
// チュートリアルプレイヤー処理 [tutoPlayer.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "playerBullet.h"
#include "playerShield.h"
#include "tutorialPlayer.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define TUTOPLAYER_SPEED			(6.0f)						//プレイヤーの移動速度					

#define TUTOPLAYER_INITIALIZED_FRAME	(90.0f/30)				//INIT状態から遷移する時間
#define TUTOPLAYER_INITIALIZED_LENGTH	(20.0f)					//INIT状態で移動する距離

#define TUTOPLAYER_MOVE_FRAME			(240)					//移動チュートリアルにかける時間

#define TUTOPLAYER_SHIELD_FRAME			(240)					//シールドチュートリアルにかける時間

#define TUTOPLAYER_ENERGY_FRAME			(240)					//エネルギーチュートリアルにかける時間

#define TUTOPLAYER_BOMB_FRAME			(240)					//ボムチュートリアルにかける時間

#define TUTOPLAYER_SHOT_FRAME			(240)					//ショットチュートリアルにかける時間

#define TUTOPLAYER_SHOTPOS_L		(D3DXVECTOR3(-20.0f, -50.0f, 0.0f))		//プレイヤーバレットの発射位置（左）
#define TUTOPLAYER_SHOTPOS_R		(D3DXVECTOR3(20.0f, -50.0f, 0.0f))		//プレイヤーバレットの発射位置（右）
#define TUTOPLAYER_SIDESHOTPOS_L	(D3DXVECTOR3(-40.0f, -20.0f, 0.0f))		//プレイヤーバレットの横発射位置（左）
#define TUTOPLAYER_SIDESHOTPOS_R	(D3DXVECTOR3(40.0f, -20.0f, 0.0f))		//プレイヤーバレットの横発射位置（右）

#define TUTOPLAYER_FIREPOS_L		(D3DXVECTOR3(-15.0f, -50.0f, 0.0f))		//バレットファイアのセット位置（左）
#define TUTOPLAYER_FIREPOS_R		(D3DXVECTOR3(15.0f, -50.0f, 0.0f))		//バレットファイアのセット位置（右）
#define TUTOPLAYER_SIDEFIREPOS_L	(D3DXVECTOR3(-35.0f, -20.0f, 0.0f))		//バレットファイアのセット位置（サブ左）
#define TUTOPLAYER_SIDEFIREPOS_R	(D3DXVECTOR3(35.0f, -20.0f, 0.0f))		//バレットファイアのセット位置（サブ右）

#define TUTOPLAYER_SHOTINTERBAL		(1)										//プレイヤーバレットの発射間隔
#define TUTOPLAYER_SHOTANGLE		(2.5f)									//プレイヤーバレットの発射角度（サブショット用）
#define TUTOPLAYER_SHOTNUM_MAX		(6)										//shotNumの最大値

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexTutoPlayer(void);							//頂点作成関数
void SetTextureTutoPlayer(int cntPattern);					// テクスチャ座標の計算処理
void SetVertexTutoPlayer(TUTOPLAYER *ptr);					// 頂点の計算処理

//各種更新処理
bool OnUpdateTutoPlayerInit(TUTOPLAYER *ptr);
bool OnUpdateTutoPlayerMove(TUTOPLAYER *ptr);
bool OnUpdateTutoPlayerInit(TUTOPLAYER *ptr);
bool OnUpdateTutoPlayerInit(TUTOPLAYER *ptr);
bool OnUpdateTutoPlayerInit(TUTOPLAYER *ptr);

//移動処理
void MoveTutoPlayer(TUTOPLAYER *ptr);

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク

TUTOPLAYER tutoPlayer;				//チュートリアルプレイヤー配列

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitTutoPlayer(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TUTOPLAYER* ptr = GetTutoPlayerAdr(0);
	int i;

	for (i = 0; i < TUTOPLAYER_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(TUTOPLAYER_INITPOS_X, TUTOPLAYER_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(TUTOPLAYER_TEXTURE_SIZE_Y, TUTOPLAYER_TEXTURE_SIZE_X);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(TUTOPLAYER_TEXTURE_SIZE_X, TUTOPLAYER_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->cntFrame = 0;

		ptr->shotInterbal = 0;
		ptr->numFireShot = 0;
		
		ptr->flgBomb = false;
		ptr->active = false;
	}

	// 頂点情報の作成
	MakeVertexTutoPlayer();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)TUTOPLAYER_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitTutoPlayer(void)
{
	if (texture != NULL)
	{	// テクスチャの開放
		texture->Release();
		texture = NULL;
	}
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateTutoPlayer(void)
{

}

/******************************************************************************
描画処理
******************************************************************************/
void DrawTutoPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TUTOPLAYER *ptr = GetTutoPlayerAdr(0);
	int i;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	//チュートリアルプレイヤーを加算合成で描画
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	for (i = 0; i < TUTOPLAYER_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//頂点座標を設定
			SetVertexTutoPlayer(ptr);

			// テクスチャ座標を設定
			SetTextureTutoPlayer(ptr->patternAnim);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//当たり判定の描画
			//DrawBoxCollider(ptr->pos + D3DXVECTOR3(TUTOPLAYER_BB_LEFT, TUTOPLAYER_BB_TOP, 0.0f), TUTOPLAYER_BB_WIDTH, TUTOPLAYER_BB_HIGHT);
#endif
		}
	}
	//描画方法を通常の方法に再設定
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexTutoPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// rhwの設定
	vertexWk[0].rhw =
		vertexWk[1].rhw =
		vertexWk[2].rhw =
		vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	return S_OK;
}

/******************************************************************************
テクスチャ座標の設定
******************************************************************************/
void SetTextureTutoPlayer(int cntPattern)
{
	// テクスチャ座標の設定
	int x = cntPattern % TUTOPLAYER_TEXTURE_DIVIDE_X;
	int y = cntPattern / TUTOPLAYER_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / TUTOPLAYER_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / TUTOPLAYER_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexTutoPlayer(TUTOPLAYER *ptr)
{
	// 頂点座標の設定
	vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius;
}

/******************************************************************************
//関数名	：TUTOPLAYER *GetTutoPlayerAdr(int pno)
//引数		：int pno：取得したいチュートリアルプレイヤーのインデックス
//戻り値	：正常な引数：チュートリアルプレイヤーのアドレス	異常な引数：NULL
//説明		：チュートリアルプレイヤーのアドレス取得関数
******************************************************************************/
TUTOPLAYER *GetTutoPlayerAdr(int pno)
{
	return &tutoPlayer;
}

/******************************************************************************
チュートリアルプレイヤー更新処理（INIT状態）
******************************************************************************/
bool OnUpdateTutoPlayerInit(TUTOPLAYER *ptr)
{
	float rad = RADIAN((float)ptr->countAnim * TUTOPLAYER_INITIALIZED_FRAME);
	ptr->pos.y -= TUTOPLAYER_INITIALIZED_LENGTH * cosf(rad);

	if (ptr->cntFrame >= TUTOPLAYER_INITIALIZED_FRAME)
	{
		ptr->cntFrame = 0;
		return true;
	}

	return false;
}

/******************************************************************************
チュートリアルプレイヤー更新処理（MOVE状態）
******************************************************************************/
bool OnUpdateTutoPlayerMove(TUTOPLAYER *ptr)
{
	MoveTutoPlayer(ptr);

	ptr->cntFrame++;
	if (ptr->cntFrame == TUTOPLAYER_MOVE_FRAME)
	{
		ptr->cntFrame = 0;
		return true;
	}
	return false;
}

/******************************************************************************
チュートリアルプレイヤー更新処理（SHOT状態）
******************************************************************************/
bool OnUpdateTutoPlayerShot(TUTOPLAYER *ptr)
{
	MoveTutoPlayer(ptr);

	ptr->cntFrame++;
	if (ptr->cntFrame == TUTOPLAYER_MOVE_FRAME)
	{
		ptr->cntFrame = 0;
		return true;
	}

	return false;
}

/******************************************************************************
チュートリアルプレイヤー更新処理（ATTACK状態）
******************************************************************************/
bool TutoPlayerAttack(TUTOPLAYER *ptr)
{
	int i;

	MoveTutoPlayer(ptr);

	//SHOTPOSからプレイヤーバレットを発射
	FirePlayerBullet(ptr->pos + TUTOPLAYER_SHOTPOS_R);
	FirePlayerBullet(ptr->pos + TUTOPLAYER_SHOTPOS_L);

	SetBulletFire(ptr->fire[0]);
	SetBulletFire(ptr->fire[1]);

	if (!GetMyButton1Down(0))
	{
		//SIDESHOTPOSからサブバレットを発射
		for (i = ptr->numFireShot; i < ptr->numFireShot + 3; i++)
		{
			FirePlayerSubBullet(ptr->pos + TUTOPLAYER_SIDESHOTPOS_L, 90 - (i + 1) * TUTOPLAYER_SHOTANGLE);
			FirePlayerSubBullet(ptr->pos + TUTOPLAYER_SIDESHOTPOS_R, 90 + (i + 1) * TUTOPLAYER_SHOTANGLE);
		}

		SetBulletFire(ptr->fire[2]);
		SetBulletFire(ptr->fire[3]);
	}

	ptr->cntFrame++;
	if (ptr->cntFrame == TUTOPLAYER_SHOT_FRAME)
	{
		ptr->cntFrame = 0;
		return true;
	}

	return false;
}

/******************************************************************************
チュートリアルプレイヤー移動処理
******************************************************************************/
void MoveTutoPlayer(TUTOPLAYER *ptr)
{
	int x = GetMyAxisX(0);
	int y = GetMyAxisY(0);

	D3DXVECTOR3 moveDir = D3DXVECTOR3((float)x, (float)y, 0.0f);
	D3DXVec3Normalize(&moveDir, &moveDir);

	ptr->pos += moveDir * TUTOPLAYER_SPEED;
}

/******************************************************************************
チュートリアルプレイヤー更新処理（SHIELD状態）
******************************************************************************/
bool OnUpdateTutoPlayerShield(TUTOPLAYER *ptr)
{
	MoveTutoPlayer(ptr);

	PLAYERSHIELD *shield = GetPlayerShieldAdr();

	if (shield->active)
	{
		ptr->cntFrame++;
	}

	if (ptr->cntFrame == TUTOPLAYER_SHIELD_FRAME)
	{
		ptr->cntFrame = 0;
		return true;
	}

	return false;
}

/******************************************************************************
チュートリアルプレイヤー更新処理（ENERGY状態）
******************************************************************************/
bool OnUpdateTutoPlayerEnergy(TUTOPLAYER *ptr)
{
	MoveTutoPlayer(ptr);

	ptr->cntFrame++;

	if (ptr->cntFrame == TUTOPLAYER_ENERGY_FRAME)
	{
		ptr->cntFrame = 0;
		return true;
	}

	return false;
}

/******************************************************************************
チュートリアルプレイヤー更新処理（BOMBER状態）
******************************************************************************/
bool OnUpdateTutoPlayerBomber(TUTOPLAYER *ptr)
{
	MoveTutoPlayer(ptr);

	if (GetMyButton0Down(0))
	{
		ptr->flgBomb = true;
	}

	if (ptr->flgBomb)

	{
		ptr->cntFrame++;
	}

	if (ptr->cntFrame == TUTOPLAYER_BOMB_FRAME)
	{
		ptr->cntFrame = 0;
		return true;
	}

	return false;
}