//=============================================================================
//
// 入力処理 [input.h]
// Author : 
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************

// プログラム分けするときに使う
#define	USE_KEYBOARD										// 宣言するとキーボードで操作可能になる
#define	USE_MOUSE											// 宣言するとマウスで操作可能になる
#define	USE_PAD												// 宣言するとパッドで操作可能になる


/* game pad情報 */
#define BUTTON_UP		0x00000001l	// 方向キー上(.IY<0)
#define BUTTON_DOWN		0x00000002l	// 方向キー下(.IY>0)
#define BUTTON_LEFT		0x00000004l	// 方向キー左(.IX<0)
#define BUTTON_RIGHT	0x00000008l	// 方向キー右(.IX>0)
#define BUTTON_A		0x00000010l	// Ａボタン(.rgbButtons[0]&0x80)
#define BUTTON_B		0x00000020l	// Ｂボタン(.rgbButtons[1]&0x80)
#define BUTTON_C		0x00000040l	// Ｃボタン(.rgbButtons[2]&0x80)
#define BUTTON_X		0x00000080l	// Ｘボタン(.rgbButtons[3]&0x80)
#define BUTTON_Y		0x00000100l	// Ｙボタン(.rgbButtons[4]&0x80)
#define BUTTON_Z		0x00000200l	// Ｚボタン(.rgbButtons[5]&0x80)
#define BUTTON_L		0x00000400l	// Ｌボタン(.rgbButtons[6]&0x80)
#define BUTTON_R		0x00000800l	// Ｒボタン(.rgbButtons[7]&0x80)
#define BUTTON_START	0x00001000l	// ＳＴＡＲＴボタン(.rgbButtons[8]&0x80)
#define BUTTON_M		0x00002000l	// Ｍボタン(.rgbButtons[9]&0x80)
#define GAMEPADMAX		4			// 同時に接続するジョイパッドの最大数をセット
#define PADBUTTON_MAX	14			// 入力に使えるボタンの最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
typedef struct
{
	//bool	button0;				//ショットボタンの入力状態
	//bool	button1;				//時間操作ボタン1の入力状態
	//bool	button2;				//時間操作ボタン2の入力状態
	//bool	pause;					//ポーズボタンの入力状態
	int		axisX;					//X軸方向の入力状態
	int		axisY;					//Y軸方向の入力状態

	int		button0Pad, button0Key;			//button0のパッドボタン定義、キーボード定義
	int		button1Pad, button1Key;			//button1のパッドボタン定義、キーボード定義
	int		button2Pad, button2Key;			//button2のパッドボタン定義、キーボード定義
	int		pausePad, pauseKey;				//ポーズボタンのパッドボタン定義、キーボード定義
	int		slowButtonPad, slowButtonKey;	//スロウボタンのパッド定義、キーボード定義

}MYINPUT;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInst, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

//---------------------------- keyboard
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardRelease(int nKey);

//---------------------------- mouse
BOOL IsMouseLeftPressed(void);      // 左クリックした状態
BOOL IsMouseLeftTriggered(void);    // 左クリックした瞬間
BOOL IsMouseRightPressed(void);     // 右クリックした状態
BOOL IsMouseRightTriggered(void);   // 右クリックした瞬間
BOOL IsMouseCenterPressed(void);    // 中クリックした状態
BOOL IsMouseCenterTriggered(void);  // 中クリックした瞬間
long GetMouseX(void);               // マウスがX方向に動いた相対値
long GetMouseY(void);               // マウスがY方向に動いた相対値
long GetMouseZ(void);               // マウスホイールが動いた相対値

//---------------------------- game pad
BOOL IsButtonPressed(int padNo,DWORD button);
BOOL IsButtonTriggered(int padNo,DWORD button);

bool GetMyButton0Down(int no);		//ボタン0（ショットボタン）の状態取得関数
bool GetMyButton1Down(int no);		//ボタン1（時間操作ボタン1）の状態取得関数
bool GetMyButton2Down(int no);		//ボタン2（時間操作ボタン2）の状態取得関数
bool GetMySlowButtonDown(int no);	

bool GetMyButton0Trigger(int no);	//ボタン0のトリガー取得関数
bool GetMyButton1Trigger(int no);	//ボタン1のトリガー取得関数
bool GetMyButton2Trigger(int no);	//ボタン2のトリガー取得関数
bool GetSlowButtonTrigger(int no);

int GetMyAxisX(int no);				//X軸方向の入力状態取得関数
int GetMyAxisY(int no);				//Y軸方向の入力状態取得関数
int GetMyAxisYTrigger(int no);		//Y軸方向のトリガー取得関数
int GetMyAxisXTrigger(int no);		//X軸方向のトリガー取得関数
int GetMyAxisYRepeat(int no);		//Y軸方向のリピート取得関数
int GetMyAxisXRepeat(int no);		//Y軸方向のリピート取得関数

bool GetMyButtonPuaseTrigger(int no);  //ポーズボタンのTrigger状態取得関数

int GetPadCount(void);				//パッドの数検出関数
#endif