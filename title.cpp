//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "title.h"
#include "input.h"
#include "transition.h"
#include "charChip.h"
#include "bgmPlayer.h"
#include "sePlayer.h"
#include "titleGear.h"
#include "bgmPlayer.h"
#include "player.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define TITLE_MINUTEHAND_ANGLE			(30.0f)			//短針が一度に回転する角度
#define TITLE_MINUTEHAND_ROTFRAME		(60)			//短針が何フレームおきに回転するか
#define TITLE_HOURHAND_ANGLE			(30.0f/TITLE_MINUTEHAND_ROTFRAME/12) //長針が1フレームに回転する角度

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexTitle(void);				//頂点作成関数
void SetTextureTitle(void);					// テクスチャ座標の計算処理
void SetTextureTitleDifficulty(void);		// テクスチャ座標の計算処理（難易度用）
void SetTextureTitleGUI(void);				// テクスチャ座標の計算処理（GUI用）

void SetVertexTitle(void);					// 頂点の計算処理（背景用）
void SetVertexTitleLogo(void);				// 頂点の計算処理（ロゴ用）
void SetVertexTitleMinuteHand(void);		// 頂点の計算処理（長針用）
void SetVertexTitleHourHand(void);			// 頂点の計算処理（短針用）
void SetVertexTitleDifficulty(void);		// 頂点の計算処理（難易度用）
void SetVertexTitleGearCenter(void);		// 頂点の計算処理（歯車用）
void SetVertexTitleGUI(void);				// 頂点の計算処理（GUI用）
void SetVertexTitleCopyRight(void);			// 頂点の計算処理（コピーライト用）

//各テクスチャ描画処理
void DrawTitleBG(void);
void DrawTitleMinuteHand(void);
void DrawTitleHourHand(void);
void DrawTitleLogo(void);
void DrawTitleDifficulty(void);
void DrawTitleGearCenter(void);
void DrawTitleGUI(void);
void DrawTitleCopyRight(void);

/*****************************************************************************
構造体定義
*****************************************************************************/
enum TITLE_TEXTURE
{
	TITLE_BACK,
	TITLE_DIAL,
	TITLE_MINUTEHAND,
	TITLE_HOURHAND,
	TITLE_LOGO,
	TITLE_DIFFICULTY,
	TITLE_GEAR,
	TITLE_GUI,
	TITLE_COPYRIGHT,
	TITLETEX_MAX
};

enum TITLE_STATE
{
	TITLE_INIT,
	TITLE_SELECT,
	TITLE_TRANSITION,
	TITLE_STATEMAX
};

enum TITLE_DIFFICULTY
{
	TITLE_EASY,
	TITLE_NORMAL,
	TITLE_HARD,
	TITLE_DEMO,
	TITLE_DIFFICULTYMAX
};
/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[TITLETEX_MAX];	// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];				//頂点情報格納ワーク

static TITLE title;									//タイトル

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitTitle(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLE* ptr = GetTitleAdr();

	ptr->pos = D3DXVECTOR3(TITLE_INITPOS_X, TITLE_INITPOS_Y, 0.0f);
	ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	ptr->angle = atan2f(TITLE_TEXTURE_SIZE_Y, TITLE_TEXTURE_SIZE_X);
	ptr->radius = D3DXVec2Length(&D3DXVECTOR2(TITLE_TEXTURE_SIZE_X, TITLE_TEXTURE_SIZE_Y));

	ptr->angleLogo = atan2f(TITLE_LOGO_SIZE_Y, TITLE_LOGO_SIZE_X);
	ptr->radiusLogo = D3DXVec2Length(&D3DXVECTOR2(TITLE_LOGO_SIZE_X, TITLE_LOGO_SIZE_Y));

	ptr->angleDifficulty = atan2f(TITLE_DIFFICULTY_SIZE_Y, TITLE_DIFFICULTY_SIZE_X);
	ptr->radiusDifficulty = D3DXVec2Length(&D3DXVECTOR2(TITLE_DIFFICULTY_SIZE_X, TITLE_DIFFICULTY_SIZE_Y));


	ptr->angleGearBig = atan2f(TITLE_GEARCENTER_SIZE_Y, TITLE_GEARCENTER_SIZE_X);
	ptr->radiusGearBig = D3DXVec2Length(&D3DXVECTOR2(TITLE_GEARCENTER_SIZE_X, TITLE_GEARCENTER_SIZE_Y));

	ptr->angleGUI = atan2f(TITLE_GUITEX_SIZE_Y, TITLE_GUITEX_SIZE_X);
	ptr->radiusGUI = D3DXVec2Length(&D3DXVECTOR2(TITLE_GUITEX_SIZE_X, TITLE_GUITEX_SIZE_Y));

	ptr->state = TITLE_SELECT;


	// 頂点情報の作成
	MakeVertexTitle();
	SetTextureTitle();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture[TITLE_BACK] = CreateTextureFromFile((LPSTR)TITLE_BGTEX_NAME, pDevice);
		texture[TITLE_DIAL] = CreateTextureFromFile((LPSTR)TITLE_ANIMTEX0_NAME, pDevice);
		texture[TITLE_MINUTEHAND] = CreateTextureFromFile((LPSTR)TITLE_ANIMTEX1_NAME, pDevice);
		texture[TITLE_HOURHAND] = CreateTextureFromFile((LPSTR)TITLE_ANIMTEX2_NAME, pDevice);
		texture[TITLE_LOGO] = CreateTextureFromFile((LPSTR)TITLE_TEXTURE_NAME, pDevice);
		texture[TITLE_DIFFICULTY] = CreateTextureFromFile((LPSTR)TITLE_DIFFICULTY_NAME, pDevice);
		texture[TITLE_GEAR] = CreateTextureFromFile((LPSTR)TITLE_GEARTEX_NAME, pDevice);
		texture[TITLE_GUI] = CreateTextureFromFile((LPSTR)TITLE_GUITEX_NAME, pDevice);
		texture[TITLE_COPYRIGHT] = CreateTextureFromFile((LPSTR)TITLE_COPYRIGHT_NAME, pDevice);

		//難易度初期化
		ptr->difficulty = DIFFICULTY_EASY;
	}

	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// テクスチャ拡大時の補間設定
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// テクスチャ縮小時の補間設定

	InitTitleGear(num);

	//FadeinBGM(BGM_TITLE, 6000);
	PlayBGM(BGM_TITLE);
	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitTitle(void)
{
	for (int i = 0; i < TITLETEX_MAX; i++)
	{
		if (texture[i] != NULL)
		{	// テクスチャの開放
			texture[i]->Release();
			texture[i] = NULL;
		}
	}

	UninitTitleGear();
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateTitle(void)
{
	TITLE *ptr = &title;

	UpdateTitleGear();

	//タイトルアニメーション
	ptr->cntFrame++;
	if (ptr->cntFrame % TITLE_MINUTEHAND_ROTFRAME == 0)
	{
		PlaySE(SE_CLOCK);
	}

	//繊維中は以降の処理を行わない
	if (ptr->state == TITLE_TRANSITION)
	{
		return;
	}

	bool flgInput = GetMyButton0Trigger(0) | GetMyButton1Trigger(0) | GetMyButton2Trigger(0);

	//最初のボタン検知
	if (flgInput && ptr->state == TITLE_INIT)
	{
		ptr->state = TITLE_SELECT;
		PlaySE(SE_DECIDE);
		return;
	}

	//難易度選択
	else if (ptr->state == TITLE_SELECT)
	{
		//横キーで難易度選択
		int x = GetMyAxisXRepeat(0);
		ptr->difficulty = WrapAround(TITLE_EASY, TITLE_DIFFICULTYMAX, ptr->difficulty + x);
		if (x != 0)
		{
			//選択時のSE再生
			PlaySE(SE_CURSOR);
		}

		//決定ボタンが押されたらチュートリアル開始
		if (flgInput && ptr->state == TITLE_SELECT)
		{
			ptr->state = TITLE_TRANSITION;
			PlaySE(SE_DECIDE);
			//StartStage(ptr->difficulty);
			SetDifficulty(ptr->difficulty);
			SetTransition(SCENE_TUTORIAL);
			InitGame(0);
			FadeoutBGM(10);

		}
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawTitle(void)
{
	SetTextureTitle();

	DrawTitleBG();
	DrawTitleGear();
	DrawTitleGearCenter();
	DrawTitleMinuteHand();
	DrawTitleHourHand();
	DrawTitleLogo();
	DrawTitleCopyRight();

	DrawTitleDifficulty();
	DrawTitleGUI();
}

/******************************************************************************
描画処理（背景用）
******************************************************************************/
void DrawTitleBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture[TITLE_BACK]);
	SetVertexTitle();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
描画処理（長針用）
******************************************************************************/
void DrawTitleHourHand(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture[TITLE_HOURHAND]);

	SetVertexTitleHourHand();

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
描画処理（短針用）
******************************************************************************/
void DrawTitleMinuteHand(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture[TITLE_MINUTEHAND]);

	SetVertexTitleMinuteHand();

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
描画処理（ロゴ用）
******************************************************************************/
void DrawTitleLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture[TITLE_LOGO]);

	SetVertexTitleLogo();

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
描画処理（難易度用）
******************************************************************************/
void DrawTitleDifficulty(void)
{
	if (title.state == TITLE_INIT)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture[TITLE_DIFFICULTY]);

	SetVertexTitleDifficulty();
	SetTextureTitleDifficulty();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
描画処理（歯車用）
******************************************************************************/
void DrawTitleGearCenter(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture[TITLE_GEAR]);

	SetVertexTitleGearCenter();

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
描画処理（GUI用）
******************************************************************************/
void DrawTitleGUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture[TITLE_GUI]);

	SetVertexTitleGUI();
	SetTextureTitleGUI();

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
描画処理（コピーライト用）
******************************************************************************/
void DrawTitleCopyRight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture[TITLE_COPYRIGHT]);

	SetVertexTitleCopyRight();

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexTitle(void)
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
void SetTextureTitle(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
テクスチャ座標の設定（難易度用）
******************************************************************************/
void SetTextureTitleDifficulty(void)
{
	TITLE *ptr = &title;

	float sizeY = 1.0f / TITLE_DIFFICULTY_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2(0.0, ptr->difficulty * sizeY);
	vertexWk[1].tex = D3DXVECTOR2(1.0, ptr->difficulty * sizeY);
	vertexWk[2].tex = D3DXVECTOR2(0.0, ptr->difficulty * sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2(1.0, ptr->difficulty * sizeY + sizeY);
}

/******************************************************************************
テクスチャ座標の設定（GUI用）
******************************************************************************/
void SetTextureTitleGUI(void)
{
	TITLE *ptr = &title;

	int y = Clamp(0, TITLE_SELECT, ptr->state);
	float sizeY = 1.0f / TITLE_GUITEX_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2(0.0, y * sizeY);
	vertexWk[1].tex = D3DXVECTOR2(1.0, y * sizeY);
	vertexWk[2].tex = D3DXVECTOR2(0.0, y * sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2(1.0, y * sizeY + sizeY);
}

/******************************************************************************
頂点座標の設定（背景用）
******************************************************************************/
void SetVertexTitle(void)
{
	vertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
}

/******************************************************************************
頂点座標の設定（ロゴ用）
******************************************************************************/
void SetVertexTitleLogo(void)
{
	static D3DXVECTOR3 pos = D3DXVECTOR3(TITLE_LOGO_INITPOS_X, TITLE_LOGO_INITPOS_Y, 0.0f);
	TITLE *ptr = &title;

	// 頂点座標の設定
	vertexWk[0].vtx.x = pos.x - cosf(ptr->angleLogo) * ptr->radiusLogo;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleLogo) * ptr->radiusLogo;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angleLogo) * ptr->radiusLogo;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleLogo) * ptr->radiusLogo;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angleLogo) * ptr->radiusLogo;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleLogo) * ptr->radiusLogo;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angleLogo) * ptr->radiusLogo;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleLogo) * ptr->radiusLogo;
}

/******************************************************************************
頂点座標の設定(短針用）
******************************************************************************/
void SetVertexTitleMinuteHand(void)
{
	static D3DXVECTOR3 pos = D3DXVECTOR3(TITLE_LOGO_INITPOS_X, TITLE_LOGO_INITPOS_Y, 0.0f);
	TITLE *ptr = &title;
	float angle = (ptr->cntFrame / TITLE_MINUTEHAND_ROTFRAME) * TITLE_MINUTEHAND_ANGLE;
	float rot = RADIAN(angle);

	// 頂点座標の設定
	vertexWk[0].vtx.x = pos.x - cosf(ptr->angleLogo + rot) * ptr->radiusLogo;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleLogo + rot) * ptr->radiusLogo;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angleLogo - rot) * ptr->radiusLogo;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleLogo - rot) * ptr->radiusLogo;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angleLogo - rot) * ptr->radiusLogo;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleLogo - rot) * ptr->radiusLogo;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angleLogo + rot) * ptr->radiusLogo;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleLogo + rot) * ptr->radiusLogo;
}

/******************************************************************************
頂点座標の設定（長針用）
******************************************************************************/
void SetVertexTitleHourHand(void)
{
	static D3DXVECTOR3 pos = D3DXVECTOR3(TITLE_LOGO_INITPOS_X, TITLE_LOGO_INITPOS_Y, 0.0f);
	TITLE *ptr = &title;
	float rot = RADIAN(TITLE_HOURHAND_ANGLE * ptr->cntFrame);

	// 頂点座標の設定
	vertexWk[0].vtx.x = pos.x - cosf(ptr->angleLogo + rot) * ptr->radiusLogo;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleLogo + rot) * ptr->radiusLogo;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angleLogo - rot) * ptr->radiusLogo;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleLogo - rot) * ptr->radiusLogo;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angleLogo - rot) * ptr->radiusLogo;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleLogo - rot) * ptr->radiusLogo;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angleLogo + rot) * ptr->radiusLogo;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleLogo + rot) * ptr->radiusLogo;
}

/******************************************************************************
頂点座標の設定（難易度用）
******************************************************************************/
void SetVertexTitleDifficulty(void)
{
	static D3DXVECTOR3 pos = D3DXVECTOR3(TITLE_DIFFICULTY_INITPOS_X, TITLE_DIFFICULTY_INITPOS_Y, 0.0f);
	TITLE *ptr = &title;

	// 頂点座標の設定
	vertexWk[0].vtx.x = pos.x - cosf(ptr->angleDifficulty) * ptr->radiusDifficulty;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleDifficulty) * ptr->radiusDifficulty;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angleDifficulty) * ptr->radiusDifficulty;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleDifficulty) * ptr->radiusDifficulty;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angleDifficulty) * ptr->radiusDifficulty;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleDifficulty) * ptr->radiusDifficulty;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angleDifficulty) * ptr->radiusDifficulty;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleDifficulty) * ptr->radiusDifficulty;
}

/******************************************************************************
頂点座標の設定（歯車用）
******************************************************************************/
void SetVertexTitleGearCenter(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(TITLE_LOGO_INITPOS_X, TITLE_LOGO_INITPOS_Y, 0.0f);
	TITLE *ptr = &title;

	//頂点座標の設定
	vertexWk[0].vtx.x = pos.x - cosf(ptr->angleGearBig) * ptr->radiusGearBig;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleGearBig) * ptr->radiusGearBig;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angleGearBig) * ptr->radiusGearBig;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleGearBig) * ptr->radiusGearBig;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angleGearBig) * ptr->radiusGearBig;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleGearBig) * ptr->radiusGearBig;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angleGearBig) * ptr->radiusGearBig;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleGearBig) * ptr->radiusGearBig;
}

/******************************************************************************
頂点座標の設定（GUI用）
******************************************************************************/
void SetVertexTitleGUI(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(TITLE_DIFFICULTY_INITPOS_X, TITLE_DIFFICULTY_INITPOS_Y, 0.0f);
	TITLE *ptr = &title;

	//頂点座標の設定
	vertexWk[0].vtx.x = pos.x - cosf(ptr->angleGUI) * ptr->radiusGUI;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleGUI) * ptr->radiusGUI;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angleGUI) * ptr->radiusGUI;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleGUI) * ptr->radiusGUI;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angleGUI) * ptr->radiusGUI;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleGUI) * ptr->radiusGUI;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angleGUI) * ptr->radiusGUI;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleGUI) * ptr->radiusGUI;
}

/******************************************************************************
頂点座標の設定（コピーライト用）
******************************************************************************/
void SetVertexTitleCopyRight(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(TITLE_COPYRIGHT_INITPOS_X, TITLE_COPYRIGHT_INITPOS_Y, 0.0f);
	TITLE *ptr = &title;

	//頂点座標の設定
	vertexWk[0].vtx = pos;
	vertexWk[1].vtx = pos + D3DXVECTOR3(TITLE_COPYRIGHT_SIZE_X, 0.0f, 0.0f);
	vertexWk[2].vtx = pos + D3DXVECTOR3(0.0f, TITLE_COPYRIGHT_SIZE_Y, 0.0f);
	vertexWk[3].vtx = pos + D3DXVECTOR3(TITLE_COPYRIGHT_SIZE_X, TITLE_COPYRIGHT_SIZE_Y, 0.0f);
}

/******************************************************************************
//関数名	：TITLE *GetTitleAdr(int pno)
//引数		：int pno：取得したいタイトルのインデックス
//戻り値	：正常な引数：タイトルのアドレス	異常な引数：NULL
//説明		：タイトルのアドレス取得関数
******************************************************************************/
TITLE *GetTitleAdr(void)
{
	return &title;
}