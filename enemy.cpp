//=============================================================================
//
// プレイヤー処理 [Enemy.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEnemy(int i);
void SetTextureEnemy( int cntPattern , int i);	// 
void SetVertexEnemy(int i);					// 頂点の計算処理

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTexture = NULL;		// テクスチャへのポインタ

static VERTEX_2D				g_vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

D3DXVECTOR3				g_posEnemy;						// ポリゴンの座標
D3DXVECTOR3				g_rotEnemy;						// ポリゴンの回転量
int						g_nCountEnemyAnim;						// アニメーションカウント
int						g_nPatternAnim;						// アニメーションパターンナンバー

float					g_fRadiusEnemy;					// ポリゴンの半径
float					g_fBaseAngleEnemy;					// ポリゴンの角度

int enemyDir;												// プレイヤーの向き

ENEMY enemy[ENEMY_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	int i;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (i = 0; i < ENEMY_MAX; i++)
	{
		//g_posEnemy = D3DXVECTOR3(200.0f, 200.0f, 0.0f);
		//g_rotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy[i].position = D3DXVECTOR3(i * 50.0f, 100.0f, 0.0f);
		enemy[i].rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//g_nCountEnemyAnim = 0;
		//g_nPatternAnim = 0;
		enemy[i].countAnim = 0;
		enemy[i].patternAnim = i;

		// 頂点情報の作成
		MakeVertexEnemy(i);

	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile( pDevice,		// デバイスのポインタ
		TEXTURE_ENEMY,						// ファイルの名前
		&g_pD3DTexture );					// 読み込むメモリのポインタ

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	if( g_pD3DTexture != NULL )
	{	// テクスチャの開放
		g_pD3DTexture->Release();

		g_pD3DTexture = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	//
	//					// アニメーション
	//enem++;

	//// アニメーションWaitチェック
	//if ((g_nCountEnemyAnim % TIME_ANIMATION_ENEMY) == 0)
	//{
	//	// パターンの切り替え
	//	g_nPatternAnim = (g_nPatternAnim + 1) % ANIM_PATTERN_NUM_ENEMY;

	//	// テクスチャ座標を設定
	//	SetTextureEnemy(g_nPatternAnim);
	//}
	int i;
	for (i = 0; i < ENEMY_MAX; i++)
	{
		enemy[i].countAnim++;

		if (enemy[i].countAnim % TIME_ANIMATION_ENEMY == 0)
		{
			enemy[i].patternAnim = (enemy[i].patternAnim + 1) % ANIM_PATTERN_NUM_ENEMY;

			SetTextureEnemy(enemy[i].patternAnim, i);
		}


		enemy[i].position.x += (float)(rand() % 11 - 5);
		enemy[i].position.y += (float)(rand() % 11 - 5);
		SetVertexEnemy(i);
	}
	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTexture);

	// ポリゴンの描画
	int i;
	for (i = 0; i < ENEMY_MAX; i++)
	{
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_ENEMY, enemy[i].vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexEnemy(int i)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定	
	SetVertexEnemy(i);	// 頂点の計算処理

	// rhwの設定
	//g_vertexWk[0].rhw =
	//g_vertexWk[1].rhw =
	//g_vertexWk[2].rhw =
	//g_vertexWk[3].rhw = 1.0f;

	enemy[i].vertexWk[0].rhw =
	enemy[i].vertexWk[1].rhw =
	enemy[i].vertexWk[2].rhw =
	enemy[i].vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	enemy[i].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	enemy[i].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	enemy[i].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	enemy[i].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	SetTextureEnemy(enemy[i].patternAnim, i);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureEnemy(int cntPattern, int i)
{
	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_ENEMY;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_ENEMY;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_ENEMY;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_ENEMY;

	enemy[i].vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	enemy[i].vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	enemy[i].vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	enemy[i].vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexEnemy(int i)
{
	// 頂点座標の設定
	enemy[i].vertexWk[0].vtx = D3DXVECTOR3(enemy[i].position.x, enemy[i].position.y, enemy[i].position.z);
	enemy[i].vertexWk[1].vtx = D3DXVECTOR3(enemy[i].position.x+TEXTURE_ENEMY_SIZE_X, enemy[i].position.y, enemy[i].position.z);
	enemy[i].vertexWk[2].vtx = D3DXVECTOR3(enemy[i].position.x, enemy[i].position.y+TEXTURE_ENEMY_SIZE_Y, enemy[i].position.z);
	enemy[i].vertexWk[3].vtx = D3DXVECTOR3(enemy[i].position.x+TEXTURE_ENEMY_SIZE_X, enemy[i].position.y+TEXTURE_ENEMY_SIZE_Y, enemy[i].position.z);
}


