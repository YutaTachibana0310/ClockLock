//=============================================================================
//
// 入力処理 [input.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_KEY_MAX			(256)

// game pad用設定値
#define DEADZONE		5000			// 各軸の25%を無効ゾーンとする
#define RANGE_MAX		1000			// 有効範囲の最大値
#define RANGE_MIN		-1000			// 有効範囲の最小値

#define MYINPUT_MAX		1				// 入力装置の最大数

#define AXIS_UP			1				//Y軸上方向
#define AXIS_DOWN		-1				//Y軸下方向
#define AXIS_RIGHT		1				//X軸右方向
#define AXIS_LEFT		-1				//X軸左方向

#define INPUT_SHORTWAIT (6)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
void UninitKeyboard(void);
HRESULT UpdateKeyboard(void);

HRESULT InitializeMouse(HINSTANCE hInst, HWND hWindow); // マウスの初期化
void UninitMouse();						// マウスの終了処理
HRESULT UpdateMouse();					// マウスの更新処理

HRESULT InitializePad(void);			// パッド初期化
//BOOL CALLBACK SearchPadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);	// パッド検査コールバック
void UpdatePad(void);
void UninitPad(void);

int GetPadAxisXPressed(int padNo);
int GetPadAxisXTriggered(int padNo);
int GetPadAxisYPressed(int padNo);
int GetPadAxisYTriggered(int padNo);

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//------------------------------- keyboard
LPDIRECTINPUT8			g_pDInput = NULL;					// IDirectInput8インターフェースへのポインタ
LPDIRECTINPUTDEVICE8	g_pDIDevKeyboard = NULL;			// IDirectInputDevice8インターフェースへのポインタ(キーボード)
BYTE					g_keyState[NUM_KEY_MAX];			// キーボードの状態を受け取るワーク
BYTE					g_keyStateTrigger[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
BYTE					g_keyStateRepeat[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
BYTE					g_keyStateRelease[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
int						g_keyStateRepeatCnt[NUM_KEY_MAX];	// キーボードのリピートカウンタ

//--------------------------------- mouse
static LPDIRECTINPUTDEVICE8 pMouse = NULL; // mouse

static DIMOUSESTATE2   mouseState;		// マウスのダイレクトな状態
static DIMOUSESTATE2   mouseTrigger;	// 押された瞬間だけON

//--------------------------------- game pad

static LPDIRECTINPUTDEVICE8	pGamePad[GAMEPADMAX] = { NULL,NULL,NULL,NULL };// パッドデバイス

static DWORD	padState[GAMEPADMAX];	// パッド情報（複数対応）
static DWORD	padTrigger[GAMEPADMAX];
static int		padCount = 0;			// 検出したパッドの数

//スティックのRepeat状態検出用
static int		padAxisXRepeat[GAMEPADMAX];
static int		padAxisYRepeat[GAMEPADMAX];
static int		lastAxisX[GAMEPADMAX];
static int		lastAxisY[GAMEPADMAX];
static int		axisXRepeatCnt[GAMEPADMAX];
static int		axisYRepeatCnt[GAMEPADMAX];

//-------------------------------- MYINPUT
static MYINPUT myInput[MYINPUT_MAX];

//=============================================================================
// 入力処理の初期化
//=============================================================================
HRESULT InitInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;
	int i;

	if (!g_pDInput)
	{
		// DirectInputオブジェクトの作成
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&g_pDInput, NULL);
	}

	// キーボードの初期化
	InitKeyboard(hInst, hWnd);

	// マウスの初期化
	InitializeMouse(hInst, hWnd);

	// パッドの初期化
	InitializePad();

	//MYINPUTの初期化
	for (i = 0; i < MYINPUT_MAX; i++)
	{
		myInput[i].button0Key = DIK_Z;
		myInput[i].button1Key = DIK_X;
		myInput[i].button2Key = DIK_C;
		myInput[i].pauseKey = DIK_P;
		myInput[i].slowButtonKey = DIK_LSHIFT;
		myInput[i].button0Pad = BUTTON_A;
		myInput[i].button1Pad = BUTTON_Z;
		myInput[i].button2Pad = BUTTON_B;
		myInput[i].pausePad = BUTTON_START;
		myInput[i].slowButtonPad = BUTTON_Y;

		//myInput[i].button0 = false;
		//myInput[i].button1 = false;
		//myInput[i].button2 = false;
		myInput[i].axisX = 0;
		myInput[i].axisY = 0;
	}

	return S_OK;
}

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void UninitInput(void)
{
	// キーボードの終了処理
	UninitKeyboard();

	// マウスの終了処理
	UninitMouse();

	// パッドの終了処理
	UninitPad();

	if (g_pDInput)
	{
		g_pDInput->Release();
		g_pDInput = NULL;
	}
}

//=============================================================================
// 入力処理の更新処理
//=============================================================================
void UpdateInput(void)
{
	int i;

	// キーボードの更新
	UpdateKeyboard();

	// マウスの更新
	UpdateMouse();

	// パッドの更新
	UpdatePad();

	//MYINPUTの更新
	for (i = 0; i < MYINPUT_MAX; i++)
	{
		//各ボタンの入力状態を判定
		//myInput[i].button0 = GetKeyboardPress(myInput[i].button0Key) || IsButtonPressed(i, myInput[0].button0Pad);
		//myInput[i].button1 = GetKeyboardPress(myInput[i].button1Key) || IsButtonPressed(i, myInput[0].button1Pad);
		//myInput[i].button2 = GetKeyboardPress(myInput[i].button2Key) || IsButtonPressed(i, myInput[0].button2Pad);
		//myInput[i].pause = GetKeyboardPress(myInput[i].pauseKey) || IsButtonPressed(i, myInput[0].pausePad);

		//方向キーとアナログスティックの入力状態を判定
		//Y方向
		if (GetKeyboardPress(DIK_UP) || IsButtonPressed(i, BUTTON_UP))
		{
			myInput[i].axisY = -1;
		}
		else if (GetKeyboardPress(DIK_DOWN) || IsButtonPressed(i, BUTTON_DOWN))
		{
			myInput[i].axisY = 1;
		}
		else
		{
			myInput[i].axisY = 0;
		}

		//X方向
		if (GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(i, BUTTON_RIGHT))
		{
			myInput[i].axisX = 1;
		}
		else if (GetKeyboardPress(DIK_LEFT) || IsButtonPressed(i, BUTTON_LEFT))
		{
			myInput[i].axisX = -1;
		}
		else
		{
			myInput[i].axisX = 0;
		}
	}
}

/******************************************************************************
//関数名	：bool GetMyButton0Down(int no)
//引数1		：int no	：取得したいmyInput[]のインデックス
//戻り値	：bool		：ボタン0（ショットボタン）の入力状態
//説明		：ボタン0の状態取得関数
******************************************************************************/
bool GetMyButton0Down(int no)
{
	return GetKeyboardPress(myInput[no].button0Key) || IsButtonPressed(no, myInput[no].button0Pad);
}

/******************************************************************************
//関数名	：bool GetMyButton1Down(int no)
//引数1		：int no	：取得したいmyInput[]のインデックス
//戻り値	：bool		：ボタン1（時間操作ボタン1）の入力状態
//説明		：ボタン1の状態取得関数
******************************************************************************/
bool GetMyButton1Down(int no)
{
	return GetKeyboardPress(myInput[no].button1Key) || IsButtonPressed(no, myInput[no].button1Pad);
}

/******************************************************************************
//関数名	：bool GetMyButton2Down(int no)
//引数1		：int no	：取得したいmyInput[]のインデックス
//戻り値	：bool		：ボタン2（時間操作ボタン2）の入力状態
//説明		：ボタン2の状態取得関数
******************************************************************************/
bool GetMyButton2Down(int no)
{
	return GetKeyboardPress(myInput[no].button2Key) || IsButtonPressed(no, myInput[no].button2Pad);
}

/******************************************************************************
//関数名	：bool GetSlowButton2Down(int no)
//引数1		：int no	：取得したいmyInput[]のインデックス
//戻り値	：bool		：スロウボタンの入力状態
//説明		：スロウボタンの状態取得関数
******************************************************************************/
bool GetMySlowButtonDown(int no)
{
	return GetKeyboardPress(myInput[no].slowButtonKey) || IsButtonPressed(no, myInput[no].slowButtonPad);
}

/******************************************************************************
//関数名	：bool GetMyButton0Trigger(int no)
//引数1		：int no	：取得したいmyInput[]のインデックス
//戻り値	：bool		：ボタン2（時間操作ボタン2）の入力状態
//説明		：ボタン2の状態取得関数
******************************************************************************/
bool GetMyButton0Trigger(int no)
{
	return GetKeyboardTrigger(myInput[no].button0Key) || IsButtonTriggered(no, myInput[no].button0Pad);
}

/******************************************************************************
//関数名	：bool GetMyButton1Trigger(int no)
//引数1		：int no	：取得したいmyInput[]のインデックス
//戻り値	：bool		：ボタン2（時間操作ボタン2）の入力状態
//説明		：ボタン2の状態取得関数
******************************************************************************/
bool GetMyButton1Trigger(int no)
{
	return GetKeyboardTrigger(myInput[no].button1Key) || IsButtonTriggered(no, myInput[no].button1Pad);
}

/******************************************************************************
//関数名	：bool GetMyButton2Trigger(int no)
//引数1		：int no	：取得したいmyInput[]のインデックス
//戻り値	：bool		：ボタン2（時間操作ボタン2）の入力状態
//説明		：ボタン2の状態取得関数
******************************************************************************/
bool GetMyButton2Trigger(int no)
{
	return GetKeyboardTrigger(myInput[no].button2Key) || IsButtonTriggered(no, myInput[no].button2Pad);
}
/******************************************************************************
//関数名	：bool GetMyButtonPuseTrigger(int no)
//引数1		：int no	：取得したいmyInput[]のインデックス
//戻り値	：bool		：ボタン2（時間操作ボタン2）の入力状態
//説明		：ボタン2の状態取得関数
******************************************************************************/
bool GetMyButtonPuaseTrigger(int no)
{
	return GetKeyboardTrigger(myInput[no].pauseKey);// || IsButtonPressed(no, myInput[no].pausePad);
}

/******************************************************************************
//関数名	：bool GetMyButtonPuseTrigger(int no)
//引数1		：int no	：取得したいmyInput[]のインデックス
//戻り値	：bool		：ボタン2（時間操作ボタン2）の入力状態
//説明		：ボタン2の状態取得関数
******************************************************************************/
bool GetSlowButtonTrigger(int no)
{
	return GetKeyboardTrigger(myInput[no].slowButtonKey) || IsButtonPressed(no, myInput[no].slowButtonPad);
}

/******************************************************************************
//関数名	：int GetMyAxisX(int no)
//引数1		：int no	：取得したいmyInput[]のインデックス
//戻り値	：int		：X軸方向の入力状態
//説明		：X軸方向の状態取得関数
******************************************************************************/
int GetMyAxisX(int no)
{
	return myInput[no].axisX;
}

/******************************************************************************
//関数名	：int GetMyAxisY(int no)
//引数1		：int no	：取得したいmyInput[]のインデックス
//戻り値	：int		：Y軸方向の入力状態
//説明		：Y軸方向の状態取得関数
******************************************************************************/
int GetMyAxisY(int no)
{
	return myInput[no].axisY;
}

/******************************************************************************
//関数名	：int GetMyAxisYTrigger(int no)
//引数1		：int no	：取得したいmyInput[]のインデックス
//戻り値	：int		：Y軸方向の入力状態
//説明		：Y軸方向の状態取得関数
******************************************************************************/
int GetMyAxisYTrigger(int no)
{
	if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(no, BUTTON_UP))
	{
		return AXIS_UP;
	}

	if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(no, BUTTON_DOWN))
	{
		return AXIS_DOWN;
	}

	else
	{
		return 0;
	}
}

/******************************************************************************
//関数名	：int GetMyAxisXTrigger(int no)
//引数1		：int no	：取得したいmyInput[]のインデックス
//戻り値	：int		：X軸方向の入力状態
//説明		：Y軸方向の状態取得関数
******************************************************************************/
int GetMyAxisXTrigger(int no)
{
	if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(no, BUTTON_RIGHT))
	{
		return AXIS_RIGHT;
	}

	if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(no, BUTTON_LEFT))
	{
		return AXIS_LEFT;
	}

	else
	{
		return 0;
	}
}

/******************************************************************************
//関数名	：int GetMyAxisXRepeat(int no)
//引数1		：int no	：取得したいmyInput[]のインデックス
//戻り値	：int		：X軸方向の入力状態
//説明		：Y軸方向の状態取得関数
******************************************************************************/
int GetMyAxisXRepeat(int no)
{
	if (padAxisXRepeat[no] != 0)
	{
		return padAxisXRepeat[no];
	}

	if (GetKeyboardRepeat(DIK_RIGHT))
	{
		return AXIS_RIGHT;
	}

	if (GetKeyboardRepeat(DIK_LEFT))
	{
		return AXIS_LEFT;
	}

	return 0;
}

/******************************************************************************
//関数名	：int GetMyAxisXTRepeat(int no)
//引数1		：int no	：取得したいmyInput[]のインデックス
//戻り値	：int		：X軸方向の入力状態
//説明		：Y軸方向の状態取得関数
******************************************************************************/
int GetMyAxisYRepeat(int no)
{
	if (padAxisYRepeat[no] != 0)
	{
		return padAxisYRepeat[no];
	}

	if (GetKeyboardRepeat(DIK_UP))
	{
		return AXIS_UP;
	}

	if (GetKeyboardRepeat(DIK_DOWN))
	{
		return AXIS_DOWN;
	}

	return 0;
}

//=============================================================================
// キーボードの初期化
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// デバイスオブジェクトを作成
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);
	if (FAILED(hr) || g_pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDIDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// キーボードの終了処理
//=============================================================================
void UninitKeyboard(void)
{
	if (g_pDIDevKeyboard)
	{
		g_pDIDevKeyboard->Release();
		g_pDIDevKeyboard = NULL;
	}
}

//=============================================================================
// キーボードの更新
//=============================================================================
HRESULT UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// 前回のデータを保存
	memcpy(keyStateOld, g_keyState, NUM_KEY_MAX);

	// デバイスからデータを取得
	hr = g_pDIDevKeyboard->GetDeviceState(sizeof(g_keyState), g_keyState);
	if (SUCCEEDED(hr))
	{
		for (int cnt = 0; cnt < NUM_KEY_MAX; cnt++)
		{
			g_keyStateTrigger[cnt] = (keyStateOld[cnt] ^ g_keyState[cnt]) & g_keyState[cnt];
			g_keyStateRelease[cnt] = (keyStateOld[cnt] ^ g_keyState[cnt]) & ~g_keyState[cnt];
			g_keyStateRepeat[cnt] = g_keyStateTrigger[cnt];

			if (g_keyState[cnt])
			{
				g_keyStateRepeatCnt[cnt]++;
				if (g_keyStateRepeatCnt[cnt] >= 20 && g_keyStateRepeatCnt[cnt] % INPUT_SHORTWAIT == 0)
				{
					g_keyStateRepeat[cnt] = g_keyState[cnt];
				}
			}
			else
			{
				g_keyStateRepeatCnt[cnt] = 0;
				g_keyStateRepeat[cnt] = 0;
			}
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		g_pDIDevKeyboard->Acquire();
	}

	return S_OK;
}

//=============================================================================
// キーボードのプレス状態を取得
//=============================================================================
bool GetKeyboardPress(int key)
{
	return (g_keyState[key] & 0x80) ? true : false;
}

//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
bool GetKeyboardTrigger(int key)
{
	return (g_keyStateTrigger[key] & 0x80) ? true : false;
}

//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
bool GetKeyboardRepeat(int key)
{
	return (g_keyStateRepeat[key] & 0x80) ? true : false;
}

//=============================================================================
// キーボードのリリ－ス状態を取得
//=============================================================================
bool GetKeyboardRelease(int key)
{
	return (g_keyStateRelease[key] & 0x80) ? true : false;
}


//=============================================================================
// マウス関係の処理
//=============================================================================
// マウスの初期化
HRESULT InitializeMouse(HINSTANCE hInst, HWND hWindow)
{
	HRESULT result;
	// デバイス作成
	result = g_pDInput->CreateDevice(GUID_SysMouse, &pMouse, NULL);
	if (FAILED(result) || pMouse == NULL)
	{
		MessageBox(hWindow, "No mouse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}
	// データフォーマット設定
	result = pMouse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(result))
	{
		MessageBox(hWindow, "Can't setup mouse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}
	// 他のアプリと協調モードに設定
	result = pMouse->SetCooperativeLevel(hWindow, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(result))
	{
		MessageBox(hWindow, "Mouse mode error", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	// デバイスの設定
	DIPROPDWORD prop;

	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;		// マウスの移動値　相対値

	result = pMouse->SetProperty(DIPROP_AXISMODE, &prop.diph);
	if (FAILED(result))
	{
		MessageBox(hWindow, "Mouse property error", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	// アクセス権を得る
	pMouse->Acquire();
	return result;
}
//---------------------------------------------------------
void UninitMouse()
{
	if (pMouse)
	{
		pMouse->Unacquire();
		pMouse->Release();
		pMouse = NULL;
	}

}
//-----------------------------------------------------------
HRESULT UpdateMouse()
{
	HRESULT result;
	// 前回の値保存
	DIMOUSESTATE2 lastMouseState = mouseState;
	// データ取得
	result = pMouse->GetDeviceState(sizeof(mouseState), &mouseState);
	if (SUCCEEDED(result))
	{
		mouseTrigger.lX = mouseState.lX;
		mouseTrigger.lY = mouseState.lY;
		mouseTrigger.lZ = mouseState.lZ;
		// マウスのボタン状態
		for (int i = 0; i < 8; i++)
		{
			mouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				mouseState.rgbButtons[i]) & mouseState.rgbButtons[i]);
		}
	}
	else	// 取得失敗
	{
		// アクセス権を得てみる
		result = pMouse->Acquire();
	}
	return result;

}

//----------------------------------------------
BOOL IsMouseLeftPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[0] & 0x80);	// 押されたときに立つビットを検査
}
BOOL IsMouseLeftTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[0] & 0x80);
}
BOOL IsMouseRightPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[1] & 0x80);
}
BOOL IsMouseRightTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[1] & 0x80);
}
BOOL IsMouseCenterPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[2] & 0x80);
}
BOOL IsMouseCenterTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[2] & 0x80);
}
//------------------
long GetMouseX(void)
{
	return mouseState.lX;
}
long GetMouseY(void)
{
	return mouseState.lY;
}
long GetMouseZ(void)
{
	return mouseState.lZ;
}
//================================================= game pad
//---------------------------------------- コールバック関数
BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	HRESULT result;

	result = g_pDInput->CreateDevice(lpddi->guidInstance, &pGamePad[padCount++], NULL);
	return DIENUM_CONTINUE;	// 次のデバイスを列挙

}
//---------------------------------------- 初期化
HRESULT InitializePad(void)			// パッド初期化
{
	HRESULT		result;
	int			i;

	padCount = 0;
	// ジョイパッドを探す
	g_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// セットしたコールバック関数が、パッドを発見した数だけ呼ばれる。

	for (i = 0; i < padCount; i++) {
		// ジョイスティック用のデータ・フォーマットを設定
		result = pGamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result))
			return false; // データフォーマットの設定に失敗

		// モードを設定（フォアグラウンド＆非排他モード）
//		result = pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
//		if ( FAILED(result) )
//			return false; // モードの設定に失敗

		// 軸の値の範囲を設定
		// X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
		// (max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
		// 差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドの性能による)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = RANGE_MIN;
		diprg.lMax = RANGE_MAX;
		// X軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 各軸ごとに、無効のゾーン値を設定する。
		// 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
		// 指定する値は、10000に対する相対値(2000なら20パーセント)。
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = DEADZONE;
		//X軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//Y軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		//ジョイスティック入力制御開始
		pGamePad[i]->Acquire();
	}

	return true;

}
//------------------------------------------- 終了処理
void UninitPad(void)
{
	for (int i = 0; i < GAMEPADMAX; i++) {
		if (pGamePad[i])
		{
			pGamePad[i]->Unacquire();
			pGamePad[i]->Release();
		}
	}

}

//------------------------------------------ 更新
void UpdatePad(void)
{
	HRESULT			result;
	DIJOYSTATE2		dijs;
	int				i;

	for (i = 0; i < padCount; i++)
	{
		DWORD lastPadState;
		lastPadState = padState[i];
		padState[i] = 0x00000000l;	// 初期化

		result = pGamePad[i]->Poll();	// ジョイスティックにポールをかける
		if (FAILED(result)) {
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = pGamePad[i]->Acquire();
		}

		result = pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// デバイス状態を読み取る
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = pGamePad[i]->Acquire();
		}

		// ３２の各ビットに意味を持たせ、ボタン押下に応じてビットをオンにする
		//* y-axis (forward)
		if (dijs.lY < 0)					padState[i] |= BUTTON_UP;
		//* y-axis (backward)
		if (dijs.lY > 0)					padState[i] |= BUTTON_DOWN;
		//* x-axis (left)
		if (dijs.lX < 0)					padState[i] |= BUTTON_LEFT;
		//* x-axis (right)
		if (dijs.lX > 0)					padState[i] |= BUTTON_RIGHT;
		//* Ａボタン
		if (dijs.rgbButtons[0] & 0x80)	padState[i] |= BUTTON_A;
		//* Ｂボタン
		if (dijs.rgbButtons[1] & 0x80)	padState[i] |= BUTTON_B;
		//* Ｃボタン
		if (dijs.rgbButtons[2] & 0x80)	padState[i] |= BUTTON_C;
		//* Ｘボタン
		if (dijs.rgbButtons[3] & 0x80)	padState[i] |= BUTTON_X;
		//* Ｙボタン
		if (dijs.rgbButtons[4] & 0x80)	padState[i] |= BUTTON_Y;
		//* Ｚボタン
		if (dijs.rgbButtons[5] & 0x80)	padState[i] |= BUTTON_Z;
		//* Ｌボタン
		if (dijs.rgbButtons[6] & 0x80)	padState[i] |= BUTTON_L;
		//* Ｒボタン
		if (dijs.rgbButtons[7] & 0x80)	padState[i] |= BUTTON_R;
		//* ＳＴＡＲＴボタン
		if (dijs.rgbButtons[8] & 0x80)	padState[i] |= BUTTON_START;
		//* Ｍボタン
		if (dijs.rgbButtons[9] & 0x80)	padState[i] |= BUTTON_M;

		// Trigger設定
		padTrigger[i] = ((lastPadState ^ padState[i])	// 前回と違っていて
			& padState[i]);					// しかも今ONのやつ

		//スティックのRepeat判定処理
		padAxisYRepeat[i] = GetPadAxisYTriggered(i);

		if (BUTTON_UP & padState[i])
		{
			axisYRepeatCnt[i]++;
			if (axisYRepeatCnt[i] >= 20 && axisYRepeatCnt[i] % INPUT_SHORTWAIT == 0)
			{
				padAxisYRepeat[i] = 1;
			}
		}
		else if (BUTTON_DOWN & padState[i])
		{
			axisYRepeatCnt[i]++;
			if (axisYRepeatCnt[i] >= 20 && axisYRepeatCnt[i] % INPUT_SHORTWAIT == 0)
			{
				padAxisYRepeat[i] = -1;
			}
		}
		else
		{
			axisYRepeatCnt[i] = 0;
			padAxisYRepeat[i] = 0;
		}

		padAxisXRepeat[i] = GetPadAxisXTriggered(i);

		if (BUTTON_RIGHT & padState[i])
		{
			axisXRepeatCnt[i]++;
			if (axisXRepeatCnt[i] >= 20 && axisXRepeatCnt[i] % INPUT_SHORTWAIT == 0)
			{
				padAxisXRepeat[i] = 1;
			}
		}
		else if (BUTTON_LEFT & padState[i])
		{
			axisXRepeatCnt[i]++;
			if (axisXRepeatCnt[i] >= 20 && axisXRepeatCnt[i] % INPUT_SHORTWAIT == 0)
			{
				padAxisXRepeat[i] = -1;
			}
		}
		else
		{
			axisXRepeatCnt[i] = 0;
			padAxisXRepeat[i] = 0;
		}

	}

}

//----------------------------------------------- 検査
BOOL IsButtonPressed(int padNo, DWORD button)
{
	return (button & padState[padNo]);
}

BOOL IsButtonTriggered(int padNo, DWORD button)
{
	return (button & padTrigger[padNo]);
}

void CheckAxisXRepeat(int padNo)
{

}

//スティックの入力検出
int GetPadAxisXTriggered(int padNo)
{
	if (IsButtonTriggered(padNo, BUTTON_RIGHT))
		return 1;

	else if (IsButtonTriggered(padNo, BUTTON_LEFT))
		return -1;

	else
		return 0;
}

int GetPadAxisYTriggered(int padNo)
{
	if (IsButtonTriggered(padNo, BUTTON_UP))
		return 1;

	else if (IsButtonTriggered(padNo, BUTTON_DOWN))
		return -1;

	else
		return 0;
}

//パッドの数検出関数
int GetPadCount(void)
{
	return padCount;
}