//=============================================================================
//
// Main処理 [main.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "bg.h"
#include "playerBullet.h"
#include "GUI.h"
#include "particle.h"
#include "bulletFire.h"
#include "enemyBullet.h"
#include "collider.h"
#include "playerGauge.h"
#include "enemyManager.h"
#include "PBulletExplosion.h"
#include "scoreItem.h"
#include "stageManager.h"
#include "playerExplosion.h"
#include "pauseMenu.h"
#include "title.h"
#include "saveData.h"
#include "transition.h"
#include "charChip.h"
#include "nameEntry.h"
#include "gameOver.h"
#include "sound.h"
#include "bgmPlayer.h"
#include "sePlayer.h"
#include "energyItem.h"
#include "gameClear.h"
#include "playerBomber.h"
#include "EnemyBulletExplosion.h"
#include "warning.h"
#include "tutorial.h"
#include "groundFire.h"
#include "playerParticle.h"
#include "enemyParticle.h"
#include "playerShield.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME			_T("AppClass")				// ウインドウのクラス名
#define WINDOW_NAME			_T("クロックロック")		// ウインドウのキャプション名

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT Init(HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

void DrawGameScene(void);
void UpdateGameScene(void);

void InitTutorialScene(void);
void DrawTutorialScene(void);
void UpdateTutorialScene(void);

#ifdef _DEBUG
void DrawDebug(void);
#endif

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3D9				g_pD3D = NULL;				// Direct3Dオブジェクト
LPDIRECT3DDEVICE9		g_pD3DDevice = NULL;		// Deviceオブジェクト(描画に必要)
D3DPRESENT_PARAMETERS	g_D3DPP;					// デバイスの設定



static int currentScene = SCENE_TITLE;				// シーン
static bool isPause = false;						// ポーズ中か否か
static int difficulty = 2;							// 難易度
static bool masterPause = false;					// スクショ撮影用ポーズフラグ
static bool demoMode = false;						// デモモードフラグ
static bool fullScreenMode = false;					// フルスクリーンモード判定

#ifdef _DEBUG
LPD3DXFONT				g_pD3DXFont = NULL;			// フォントへのポインタ
int						g_nCountFPS;				// FPSカウンタ
#endif

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	srand((unsigned)time(NULL));

	// 時間計測用
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND		hWnd;
	MSG			msg;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		50, //CW_USEDEFAULT,																	// ウィンドウの左座標
		0, //CW_USEDEFAULT,																		// ウィンドウの上座標
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,										// ウィンドウ横幅
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// ウィンドウ縦幅
		NULL,
		NULL,
		hInstance,
		NULL);

	// DirectXの初期化(ウィンドウを作成してから行う)
	if (FAILED(Init(hWnd, true)))
	{
		return -1;
	}

	// 入力処理の初期化
	InitInput(hInstance, hWnd);

	// フレームカウント初期化
	timeBeginPeriod(1);	// 分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;

	// ウインドウの表示(Init()の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();					// システム時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1秒ごとに実行
			{
#ifdef _DEBUG
				g_nCountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPSを測定した時刻を保存
				dwFrameCount = 0;							// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;	// 処理した時刻を保存

				Update();			// 更新処理
				Draw();				// 描画処理

				dwFrameCount++;		// 処理回数のカウントを加算
			}
		}
	}

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:					// [ESC]キーが押された
			DestroyWindow(hWnd);		// ウィンドウを破棄するよう指示する
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//液晶サイズを取得
	int screenX = GetSystemMetrics(SM_CXSCREEN);
	int screenY = GetSystemMetrics(SM_CYSCREEN);

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));				// ワークをゼロクリア

	d3dpp.BackBufferCount = 1;						// バックバッファの数

	if (bWindow)
	{
		d3dpp.BackBufferWidth = SCREEN_WIDTH;			// ゲーム画面サイズ(幅)ウィンドウモード
		d3dpp.BackBufferHeight = SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)ウィンドウモード
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;		// バックバッファのフォーマットは現在設定されているものを使う
	}
	else
	{
		d3dpp.BackBufferWidth = screenX;
		d3dpp.BackBufferHeight = screenY;
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	}
	
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.Windowed = bWindow;						// ウィンドウモード
	d3dpp.EnableAutoDepthStencil = TRUE;			// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;		// デプスバッファとして16bitを使う
	//d3dpp.BackBufferFormat = d3ddm.Format;			// カラーモードの指定

	if (bWindow)
	{// ウィンドウモード
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;					// バックバッファ
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{// フルスクリーンモード
		d3dpp.BackBufferFormat = D3DFMT_R5G6B5;					// バックバッファ
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,							// ディスプレイアダプタ
		D3DDEVTYPE_HAL,								// ディスプレイタイプ
		hWnd,										// フォーカスするウインドウへのハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING,		// デバイス作成制御の組み合わせ
		&d3dpp,										// デバイスのプレゼンテーションパラメータ
		&g_pD3DDevice)))							// デバイスインターフェースへのポインタ
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダリングステートパラメータの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// カリングを行わない
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートパラメータの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// テクスチャ拡大時の補間設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// テクスチャ縮小時の補間設定

	// 頂点フォーマットの設定
	g_pD3DDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャブレンディングの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG0, D3DTA_TEXTURE);
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

#ifdef _DEBUG
	// 情報表示用フォントを設定
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &g_pD3DXFont);
#endif

	//セーブデータ初期化
	InitSaveData(0);

	//サウンド初期化
	InitSound(hWnd);
	InitBgmPlayer(0);
	InitSEplayer(0);

	//エネミー関連初期化
	InitStageManager(0);
	InitEnemyBullet(0);
	InitEnemyManager(0);
	InitEnemyBulletExplosion(0);

	//plyaer関連初期化
	InitPlayer(0);
	InitPlayerGauge(0);
	InitPBulletExplosion(0);
	InitScoreItem(0);
	InitPlayerExplosion(0);
	InitBulletFire(0);
	InitPlayerBomber(0);
	InitEnergyItem(0);
	InitPlayerBullet(0);
	InitPlayerShield(0);

	//エフェクト初期化
	InitTransition(0);
	InitBG(0);
	InitParticle(0);

	//システム初期化
	InitGUI(0);
	InitMyLibrary(0);
	InitPauseMenu(0);
	InitTitle(0);
	InitCharChip(0);
	InitNameEntry(0);
	InitGameOver(0);
	InitGameClear(0);
	InitWarning(0);
	InitTutorial(0);
	InitGroundFire(0);
	
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	if (g_pD3DDevice != NULL)
	{// デバイスの開放
		g_pD3DDevice->Release();
	}

	if (g_pD3D != NULL)
	{// Direct3Dオブジェクトの開放
		g_pD3D->Release();
	}

	//セーブデータ保存
	UninitSaveData();

	//エネミー関連終了処理
	UninitPlayer();
	UninitStageManager();
	UninitPlayerBullet();
	UninitTransition();
	UninitEnemyBullet();
	UninitEnemyManager();
	UninitEnemyBulletExplosion();

	//player終了処理
	UninitBulletFire();
	UninitPlayerGauge();
	UninitPBulletExplosion();
	UninitScoreItem();
	UninitPlayerExplosion();
	UninitEnergyItem();
	UninitPlayerBomber();
	UninitPlayerShield();

	//エフェクト終了処理
	UninitParticle();
	UninitGroundFire();

	//システム終了
	UninitBG();
	UninitGUI();
	UninitMyLibrary();
	UninitPauseMenu();
	UninitTitle();
	UninitCharChip();
	UninitNameEntry();
	UninitGameOver();
	UninitGameClear();
	UninitWarning();
	UninitTutorial();

	// 入力処理の終了処理
	UninitInput();

	//サウンド終了処理
	UninitBgmPlayer();
	UninitSEplayer();
	UninitSound();
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	UpdateInput();

	//スクリーンショット撮影用ポーズ
	if (GetKeyboardTrigger(DIK_V))
	{
		masterPause = !masterPause;
	}
	if (masterPause)
	{
		return;
	}
	

	// 入力の更新処理
	UpdateTransition();
	UpdateBgmPlayer();
	UpdateSEplayer();

	switch (currentScene)
	{
	case SCENE_TITLE:			//タイトル画面更新処理
		UpdateTitle();
		break;

	case SCENE_TUTORIAL:		//チュートリアル画面更新処理
		UpdateTutorialScene();
		break;

	case SCENE_GAME:			//ゲーム画面更新処理
		if (GetMyButtonPuaseTrigger(0))
		{
			//ポーズ処理
			Pause(true);
			break;
		}

		UpdateGameScene();
		break;

	case SCENE_PAUSE:			//ポーズ画面更新処理
		UpdatePauseMenu();
		break;

	case SCENE_GAMEOVER:		//ゲームオーバー画面更新処理
		UpdateGameScene();
		UpdateGameOver();
		break;

	case SCENE_CLEAR:			//クリア画面更新処理
		UpdateGameScene();
		UpdateGameClear();
		break;

	case SCENE_NAMEENTRY:		//ネームエントリー画面更新処理
		UpdateNameEntry();
		break;
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		switch (currentScene)
		{
		case SCENE_TITLE:
			DrawTitle();
			break;

		case SCENE_TUTORIAL:
			DrawTutorialScene();
			break;

		case SCENE_GAME:
			DrawGameScene();
			DrawGUI();
			break;

		case SCENE_PAUSE:
			DrawGameScene();
			DrawPauseMenu();
			DrawGUI();
			break;

		case SCENE_CLEAR:
			DrawGameScene();
			DrawGameClear();
			DrawGUI();
			break;

		case SCENE_GAMEOVER:
			DrawGameScene();
			DrawGameOver();
			DrawGUI();
			break;

		case SCENE_NAMEENTRY:
			DrawNameEntry();
			break;
		}

		DrawTransition();

#ifdef _DEBUG
		// FPS表示
		DrawDebug();
#endif

		// Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// デバイス取得関数
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return(g_pD3DDevice);
}

/******************************************************************************
//関数名	：LPDIRECT3DTEXTURE9 CreateTextureFromFile(LPSTR szName, LPDIRECT3DDEVICE9 lpD3DDevice)
//引数1		：LPSTR szName：読み込みたいファイル名
//引数2		：LPDIRECT3DDEVICE9 lpD3DDevice：デバイスオブジェクト
//戻り値	：読み込んだテクスチャへのポインタ
//説明		：サイズを指定してテクスチャファイルを読み込む関数
******************************************************************************/
LPDIRECT3DTEXTURE9 CreateTextureFromFile(LPSTR szName, LPDIRECT3DDEVICE9 lpD3DDevice)
{
	HRESULT             hRet;

	//戻り値のテクスチャ
	LPDIRECT3DTEXTURE9  lpTex = NULL;
	if (lpD3DDevice) {

		//D3DXGetImageInfoFromFileで使用する画像ファイルの情報
		D3DXIMAGE_INFO      iinfo;

		//画像ファイルの情報を取得
		hRet = D3DXGetImageInfoFromFile(szName, &iinfo);
		if (hRet == D3D_OK)
		{
			//テクスチャをファイルから作成
			hRet = D3DXCreateTextureFromFileEx(
				lpD3DDevice,
				szName,
				iinfo.Width, iinfo.Height,			//取得した画像ファイルのサイズを指定する
				1, 0,
				D3DFMT_A8R8G8B8,
				D3DPOOL_MANAGED,
				D3DX_FILTER_NONE,
				D3DX_FILTER_NONE,
				0xFF000000,
				NULL, NULL,
				&lpTex);
		}
	}
	return lpTex;
}

//=============================================================================
//ポーズ切り替え関数
//=============================================================================
void Pause(bool state)
{
	isPause = state;
	
	if (state)
	{
		currentScene = SCENE_PAUSE;
		SetPauseMenuState(PAUSEMENU_OPEN);
	}
}

//=============================================================================
//シーン切り替え関数
//=============================================================================
void SetScene(int scene)
{
	currentScene = scene;

	switch (scene)
	{
	case SCENE_TITLE:
		InitTitle(1);
		PlayBGM(BGM_TITLE);
		break;

	case SCENE_GAME:
		InitGame(0);
		PlayBGM(BGM_STAGE1);
		break;

	case SCENE_NAMEENTRY:
		InitNameEntry(1);
		PlayBGM(BGM_NAMEENTRY);
		break;

	case SCENE_TUTORIAL:
		InitTutorialScene();
		PlayBGM(BGM_TUTORIAL);
		//チュートリアルBGM再生
		break;
	}
}

//=============================================================================
//ゲーム再開関数
//=============================================================================
void ResumeGame()
{
	//PlayBGM(BGM_STAGE1);
	currentScene = SCENE_GAME;
}

//=============================================================================
//ステージ開始前の初期化処理
//=============================================================================
void InitGame(int num)
{
	//difficulty = num;

	InitStageManager(1);
	InitBG(1);
	InitGUI(1);
	InitPlayer(1);
	InitPlayerBullet(1);
	InitPlayerBomber(1);
	InitParticle(1);
	InitBulletFire(1);
	InitPlayerGauge(1);
	InitEnemyBullet(1);
	InitPBulletExplosion(1);
	InitEnemyManager(1);
	InitScoreItem(1);
	InitPlayerExplosion(1);
	InitEnergyItem(1);
	InitGameOver(1);
	InitEnemyBulletExplosion(1);
	InitWarning(1);
	InitEnemyBullet(1);
	InitGroundFire(1);
	InitPlayerShield(1);

	//SetTransition(SCENE_GAME);
}

//=============================================================================
//ゲームシーン描画処理
//=============================================================================
void DrawGameScene(void)
{
	DrawBG();
	DrawGroundFire();
	DrawStageManager();
	DrawPlayerBomber();
	DrawEnemyManager();
	DrawPlayerBullet();
	DrawScoreItem();
	DrawParticle();
	DrawPlayerGauge();
	DrawPBulletExplosion();
	DrawBulletFire();
	DrawPlayer();
	DrawPlayerShield();
	DrawEnergyItem();
	DrawPlayerExplosion();
	DrawEnemyBulletExplosion();

	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	DrawEnemyParticle();
	DrawPlayerParticle();
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	DrawEnemyBullet();
	DrawParameter();
	DrawWarning();
	//GUI描画処理
	DrawBossGauge();
	DrawEnemyGauge();
	DrawGameOver();
}

//=============================================================================
//ゲームシーン更新処理
//=============================================================================
void UpdateGameScene(void)
{
	UpdateStageManager();
	UpdateBG();
	UpdateGroundFire();
	UpdatePlayerBullet();
	UpdateParticle();
	UpdateBulletFire();
	UpdatePlayer();
	UpdatePlayerShield();
	UpdatePlayerGauge();
	UpdateEnemyBullet();
	UpdateEnemyManager();
	UpdateScoreItem();
	UpdateEnergyItem();
	UpdatePlayerBomber();
	UpdatePlayerExplosion();
	UpdateEnemyBulletExplosion();
	UpdateWarning();

	//GUIの更新処理
	UpdateGUI();
	UpdatePBulletExplosion();
	UpdateCollider();
}

//=============================================================================
//チュートリアルシーン初期化処理
//=============================================================================
void InitTutorialScene(void)
{
	InitPlayerBullet(1);
	InitParticle(1);
	InitBulletFire(1);
	InitPlayer(2);
	InitPlayerGauge(1);
	InitEnemyBullet(1);
	InitScoreItem(1);
	InitPlayerBomber(1);
	InitPlayerExplosion(1);
	InitEnemyBulletExplosion(1);
	InitGUI(1);
	InitPBulletExplosion(1);
	InitEnemyBullet(1);
	InitWarning(1);
	InitTutorial(1);
	InitCollider();
}

//=============================================================================
//チュートリアルシーン更新処理
//=============================================================================
void UpdateTutorialScene(void)
{
	UpdatePlayerBullet();
	UpdateParticle();
	UpdateBulletFire();
	UpdatePlayer();
	UpdatePlayerGauge();
	UpdateEnemyBullet();
	UpdateScoreItem();
	UpdatePlayerBomber();
	UpdatePlayerExplosion();
	UpdateEnemyBulletExplosion();
	UpdateGUI();
	UpdatePBulletExplosion();
	UpdateTutorial();
	UpdateCollider();
}

//=============================================================================
//チュートリアルシーン描画処理
//=============================================================================
void DrawTutorialScene(void)
{
	DrawTutorialBG();
	DrawTimeEffect();
	DrawPlayerBomber();
	DrawPlayerBullet();
	DrawParticle();
	DrawBulletFire();
	DrawPlayerGauge();
	DrawPlayer();
	DrawEnemyBullet();
	DrawScoreItem();
	DrawPlayerExplosion();
	DrawEnemyBulletExplosion();
	DrawPBulletExplosion();

	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	DrawEnemyParticle();
	DrawPlayerParticle();
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	DrawParameter();
	DrawGUI();
	DrawTutorial();
	DrawCollider();


}

//=============================================================================
//難易度取得関数
//=============================================================================
int GetDifficulty(void)
{
	return difficulty;
}

//=============================================================================
//難易度取得関数
//=============================================================================
void SetDifficulty(int n)
{
	//通常の難易度設定
	if (n < DIFFICULTY_MAX)
	{
		difficulty = n;
		demoMode = false;
	}
	//デモ用難易度
	else
	{
		difficulty = DIFFICULTY_EASY;
		demoMode = true;
	}
}

//=============================================================================
//シーン取得関数
//=============================================================================
int GetCurrentScene(void)
{
	return currentScene;
}

//=============================================================================
//デモモード判定関数
//=============================================================================
bool isDemoMode(void)
{
	return demoMode;
}

//=============================================================================
//ディスプレイモード変更関数
//=============================================================================
void ChangeDisplayMode(void)
{
	fullScreenMode = !fullScreenMode;

	//フルスクリーンモードに設定
	if (fullScreenMode)
	{
	
	}
}


#ifdef _DEBUG
//=============================================================================
// デバッグ表示処理
//=============================================================================
void DrawDebug(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	TCHAR str[256];

	wsprintf(str, _T("FPS:%d\nEnemyBullet:%d\nScoreItem:%d\nBGM:%d"), g_nCountFPS, GetEnemyBulletCnt(), GetActiveItemCount(), (int)GetBgmPlayerAdr()->bgmVolume);

	// テキスト描画
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}
#endif