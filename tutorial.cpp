//=============================================================================
//
// チュートリアル処理 [tutorial.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "bg.h"
#include "tutorial.h"
#include "tutorialPlayer.h"
#include "bulletFire.h"
#include "playerBullet.h"
#include "tutorialEnemy.h"
#include "player.h"
#include "input.h"
#include "transition.h"
#include "bgmPlayer.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define TUTORIAL_BG_SCROLLSPEED		(-0.003f)		//背景のスクロールスピード
#define TUTORIAL_GUIDE_FADEFRAME	(60)			//ガイドがフェードする時間
#define TUTORIAL_GUIDE_VIEWFRAME	(360)			//ガイドを表示する時間
#define TUTORIAL_SKIP_COUNT			(30)			//このフレーム数、スペースが押されたらスキップ
#define TUTORIAL_END_WAIT			(120)			//チュートリアル終了後の待機フレーム

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
void SetVertexTutorialGuide(void);			//頂点座標設定処理（ガイド用）
void SetVertexTutorialChar(void);			//頂点座標設定処理（TUTORIAL用）
void SetVertexTutorialBG(void);				//頂点座標設定処理（背景用）
void SetVertexTutorialSkip(void);			//頂点座標設定処理（スキップ用）

void SetTextureTutorialGuide(void);			//テクスチャ座標設定処理（ガイド用）
void SetTextureTutorialChar(void);			//テクスチャ座標設定処理（TUTORIAL用）
void SetTextureTutorialBG(void);			//テクスチャ座標設定処理（背景用）

HRESULT MakeVertexTutorial(void);			//頂点作成処理

void SetDiffuseTutorialGuide(void);			//ディフューズ設定処理

/*****************************************************************************
構造体定義
*****************************************************************************/
//チュートリアルテクスチャ通し番号
enum TUTORIAL_TEXTURE
{
	TUTORIAL_BG,
	TUTORIAL_GUIDE,
	TUTORIAL_CHAR,
	TUTORIAL_SKIP,
	TUTORIAL_SKIPPAD,
	TUTORIAL_GUIDEPAD,
	TUTORIAL_TEXMAX
};

//チュートリアルのガイド通し番号
enum TUTORIAL_GUIDEINDEX
{
	TUTORIAL_ATTACKGUIDE,
	TUTORIAL_LOCKGUIDE1,
	TUTORIAL_LOCKGUIDE2,
	TUTORIAL_BOMBERGUIDE,
	TUTORIAL_ITEMGUIDE,
	TUTORIAL_GUIDEMAX
};

//チュートリアルのガイド表示状態
enum TUTORIAL_GUIDESTATE
{
	TUTORIAL_GUIDE_FADEIN,
	TUTORIAL_GUIDE_NOTFADE,
	TUTORIAL_GUIDE_FADEOUT,
	TUTORIAL_GUIDE_STATEMAX
};
/*****************************************************************************
グローバル変数
*****************************************************************************/
static TUTORIAL tutorial;								//チュートリアル構造体
static LPDIRECT3DTEXTURE9 texture[TUTORIAL_TEXMAX];		//テクスチャ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報
static VERTEX_2D GUIvertexWk[NUM_VERTEX];				//ガイド用の頂点情報

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitTutorial(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TUTORIAL *ptr = &tutorial;

	ptr->angleGuide = atan2f(TUTORIAL_TEXTURE_SIZE_Y, TUTORIAL_TEXTURE_SIZE_X);
	ptr->radiusGuide = D3DXVec2Length(&D3DXVECTOR2(TUTORIAL_TEXTURE_SIZE_X, TUTORIAL_TEXTURE_SIZE_Y));

	ptr->angleChar = atan2f(TUTORIAL_CHAR_SIZE_Y, TUTORIAL_CHAR_SIZE_X);
	ptr->radiusChar = D3DXVec2Length(&D3DXVECTOR2(TUTORIAL_CHAR_SIZE_X, TUTORIAL_CHAR_SIZE_Y));

	ptr->bgOffset = 0.0f;
	ptr->guideState = TUTORIAL_GUIDE_FADEIN;
	ptr->guideAlpha = 0.0f;

	ptr->buttonCount = 0;
	ptr->cntFrame = 0;
	ptr->tutorialIndex = 0;

	//テクスチャブレンディングを設定
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	MakeVertexTutorial();

	if (num == 0)
	{
		texture[TUTORIAL_BG] = CreateTextureFromFile((LPSTR)TUTORIAL_BGTEX_NAME, pDevice);
		texture[TUTORIAL_GUIDE] = CreateTextureFromFile((LPSTR)TUTORIAL_TEXTURE_NAME, pDevice);
		texture[TUTORIAL_CHAR] = CreateTextureFromFile((LPSTR)TUTORIAL_CHARTEX_NAME, pDevice);
		texture[TUTORIAL_SKIP] = CreateTextureFromFile((LPSTR)TUTORIAL_SKIPTEX_NAME, pDevice);
		texture[TUTORIAL_GUIDEPAD] = CreateTextureFromFile((LPSTR)TUTORIAL_PADTEX_NAME, pDevice);
		texture[TUTORIAL_SKIPPAD] = CreateTextureFromFile((LPSTR)TUTORIAL_SKIPPADTEX_NAME, pDevice);
	}

	InitTutorialEnemy(num);

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitTutorial(void)
{
	//テクスチャの開放
	for (int i = 0; i < TUTORIAL_TEXMAX; i++)
	{
		if (texture[i] != NULL)
		{
			texture[i]->Release();
			texture[i] = NULL;
		}
	}
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateTutorial(void)
{
	TUTORIAL *ptr = &tutorial;
	PLAYER *player = GetPlayerAdr(0);

	//チュートリアルエネミー更新処理
	UpdateTutorialEnemy();

	ptr->cntFrame++;

	//背景スクロール
	ptr->bgOffset += TUTORIAL_BG_SCROLLSPEED;
	if (ptr->bgOffset > 1.0f)
	{
		ptr->bgOffset -= 1.0f;
	}

	//スペースキーが一定時間押され続けていたらチュートリアルスキップ
	if (GetKeyboardPress(DIK_SPACE) || IsButtonPressed(0, BUTTON_M))
	{
		ptr->buttonCount++;
		if (ptr->buttonCount == TUTORIAL_SKIP_COUNT)
		{
			DisableTutorialEnemy();
			SetTransition(SCENE_GAME);
		}
	}
	else
	{
		//キーが押されていなかったらカウントリセット
		ptr->buttonCount = 0;
	}

	//チュートリアルが最後まで終了し時間経過したらゲームスタート
	if (ptr->tutorialIndex == TUTORIAL_GUIDEMAX && ptr->cntFrame == TUTORIAL_END_WAIT)
	{
		DisableTutorialEnemy();
		SetTransition(SCENE_GAME);
	}

	//チュートリアルが終了していたら以下の処理は行わない
	if (ptr->tutorialIndex == TUTORIAL_GUIDEMAX)
	{
		return;
	}

	switch (ptr->guideState)
	{
	case TUTORIAL_GUIDE_FADEIN:
		ptr->guideAlpha += 1.0f;
		//指定時間が経過していたらフェードイン終了
		if (ptr->cntFrame == TUTORIAL_GUIDE_FADEFRAME)
		{
			ptr->guideState = TUTORIAL_GUIDE_NOTFADE;
			ptr->cntFrame = 0;
		}
		break;

	case TUTORIAL_GUIDE_NOTFADE:
		//指定時間が経過していたらフェードアウト開始
		if (ptr->cntFrame == TUTORIAL_GUIDE_VIEWFRAME)
		{
			ptr->guideState = TUTORIAL_GUIDE_FADEOUT;
			ptr->cntFrame = 0;
		}
		break;

	case TUTORIAL_GUIDE_FADEOUT:
		//指定時間が経過していたらガイド切り替え
		ptr->guideAlpha -= 1.0f;
		if (ptr->cntFrame == TUTORIAL_GUIDE_FADEFRAME)
		{
			ptr->guideState = TUTORIAL_GUIDE_FADEIN;
			ptr->cntFrame = 0;

			ptr->tutorialIndex++;

			//チュートリアル進行度によってプレイヤーの機能ロック解除
			if (ptr->tutorialIndex == TUTORIAL_LOCKGUIDE1)
			{
				player->lockTimeEffect = false;
			}
			else if (ptr->tutorialIndex == TUTORIAL_BOMBERGUIDE)
			{
				player->lockBomber = false;
			}
		}
		break;
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TUTORIAL *ptr = &tutorial;

	DrawTutorialEnemy();

	//TUTORIAL文字描画
	pDevice->SetTexture(0, texture[TUTORIAL_CHAR]);
	SetVertexTutorialChar();
	SetTextureTutorialChar();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//スキップ描画
	pDevice->SetTexture(0, texture[(GetPadCount == 0) ? TUTORIAL_SKIP : TUTORIAL_SKIPPAD]);
	SetVertexTutorialSkip();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//ガイド描画(tutorialIndexがMAX未満であれば)
	if (ptr->tutorialIndex < TUTORIAL_GUIDEMAX)
	{
		LPDIRECT3DTEXTURE9 tex = (GetPadCount() > 0) ? texture[TUTORIAL_GUIDEPAD] : texture[TUTORIAL_GUIDE];
		pDevice->SetTexture(0, tex);

		SetVertexTutorialGuide();
		SetTextureTutorialGuide();
		SetDiffuseTutorialGuide();
		
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, GUIvertexWk, sizeof(VERTEX_2D));
	}
}

/******************************************************************************
描画処理(背景)
******************************************************************************/
void DrawTutorialBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->SetTexture(0, texture[TUTORIAL_BG]);
	SetVertexTutorialBG();
	SetTextureTutorialBG();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
テクスチャ座標の設定（ガイド用）
******************************************************************************/
void SetTextureTutorialGuide(void)
{
	TUTORIAL *ptr = &tutorial;

	int y = ptr->tutorialIndex;
	float sizeY = 1.0f / TUTORIAL_DIVIDE_Y;

	GUIvertexWk[0].tex = D3DXVECTOR2(0.0f, y * sizeY);
	GUIvertexWk[1].tex = D3DXVECTOR2(1.0f, y * sizeY);
	GUIvertexWk[2].tex = D3DXVECTOR2(0.0f, y * sizeY + sizeY);
	GUIvertexWk[3].tex = D3DXVECTOR2(1.0f, y * sizeY + sizeY);
}

/******************************************************************************
テクスチャ座標の設定（TUTORIAL用）
******************************************************************************/
void SetTextureTutorialChar(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
テクスチャ座標の設定（背景用）
******************************************************************************/
void SetTextureTutorialBG(void)
{
	TUTORIAL *ptr = &tutorial;

	vertexWk[0].tex = D3DXVECTOR2(0.0f, ptr->bgOffset);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, ptr->bgOffset);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, ptr->bgOffset + 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, ptr->bgOffset + 1.0f);
}

/******************************************************************************
頂点座標の設定（ガイド用）
******************************************************************************/
void SetVertexTutorialGuide(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(TUTORIAL_INITPOS_X, TUTORIAL_INITPOS_Y, 0.0f);
	TUTORIAL *ptr = &tutorial;

	GUIvertexWk[0].vtx.x = pos.x - cosf(ptr->angleGuide) * ptr->radiusGuide;
	GUIvertexWk[0].vtx.y = pos.y - sinf(ptr->angleGuide) * ptr->radiusGuide;
	GUIvertexWk[1].vtx.x = pos.x + cosf(ptr->angleGuide) * ptr->radiusGuide;
	GUIvertexWk[1].vtx.y = pos.y - sinf(ptr->angleGuide) * ptr->radiusGuide;
	GUIvertexWk[2].vtx.x = pos.x - cosf(ptr->angleGuide) * ptr->radiusGuide;
	GUIvertexWk[2].vtx.y = pos.y + sinf(ptr->angleGuide) * ptr->radiusGuide;
	GUIvertexWk[3].vtx.x = pos.x + cosf(ptr->angleGuide) * ptr->radiusGuide;
	GUIvertexWk[3].vtx.y = pos.y + sinf(ptr->angleGuide) * ptr->radiusGuide;
}

/******************************************************************************
頂点座標の設定（TUTORIAL用）
******************************************************************************/
void SetVertexTutorialChar(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(TUTORIAL_CHAR_INITPOS_X, TUTORIAL_CHAR_INITPOS_Y, 0.0f);
	TUTORIAL *ptr = &tutorial;

	vertexWk[0].vtx.x = pos.x - cosf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleChar) * ptr->radiusChar;
}

/******************************************************************************
頂点座標の設定（TUTORIAL用）
******************************************************************************/
void SetVertexTutorialBG(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(TUTORIAL_CHAR_INITPOS_X, TUTORIAL_CHAR_INITPOS_Y, 0.0f);

	vertexWk[0].vtx = D3DXVECTOR3(BG_POS_X, 0.0f, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(BG_POS_X + BG_TEXTURESIZE_X, 0.0f, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(BG_POS_X, BG_TEXTURESIZE_Y, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(BG_POS_X + BG_TEXTURESIZE_X, BG_TEXTURESIZE_Y, 0.0f);
}

/******************************************************************************
頂点情報作成
******************************************************************************/
HRESULT MakeVertexTutorial(void)
{
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

	// rhwの設定
	GUIvertexWk[0].rhw =
		GUIvertexWk[1].rhw =
		GUIvertexWk[2].rhw =
		GUIvertexWk[3].rhw = 1.0f;

	// 反射光の設定
	GUIvertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	GUIvertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	GUIvertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	GUIvertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

	return S_OK;
}

/******************************************************************************
ディフューズ設定処理
******************************************************************************/
void SetDiffuseTutorialGuide(void)
{
	TUTORIAL *ptr = &tutorial;
	int alpha = Clamp(0, 255, (int)(255 * ptr->guideAlpha / TUTORIAL_GUIDE_FADEFRAME));

	GUIvertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	GUIvertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	GUIvertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	GUIvertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
}

/******************************************************************************
頂点座標設定処理（スキップ文字用）
******************************************************************************/
void SetVertexTutorialSkip(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(TUTORIAL_SKIPTEX_INITPOS_X, TUTORIAL_SKIPTEX_INITPOS_Y, 0.0f);

	vertexWk[0].vtx = D3DXVECTOR3(pos.x, pos.y, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + TUTORIAL_SKIPTEX_SIZE_X, pos.y, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x, pos.y + TUTORIAL_SKIPTEX_SIZE_Y, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + TUTORIAL_SKIPTEX_SIZE_X, pos.y + TUTORIAL_SKIPTEX_SIZE_Y, 0.0f);
}