//=============================================================================
//
// タイトルヘッダ [Title.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_


// マクロ定義
#define TITLE_TEXTURE_NAME	_T("data/TEXTURE/bg/titlelogo.png")				//タイトル背景のテクスチャ
#define TITLE_ANIMTEX0_NAME	_T("data/TEXTURE/bg/title00.png")				//タイトルアニメーション用のテクスチャ
#define TITLE_ANIMTEX1_NAME	_T("data/TEXTURE/bg/title01.png")				//タイトルアニメーション用のテクスチャ
#define TITLE_ANIMTEX2_NAME _T("data/TEXTURE/bg/title02.png")				//タイトルアニメーション用のテクスチャ
#define TITLE_BGTEX_NAME		_T("data/TEXTURE/GUI/titleBG.png")		//タイトル背景のテクスチャ
#define TITLE_DIFFICULTY_NAME	_T("data/TEXTURE/GUI/difficultySelect.png")	//難易度選択のテクスチャ
#define TITLE_GEARTEX_NAME	_T("data/TEXTURE/bg/gear.png")					//歯車のテクスチャ
#define TITLE_GUITEX_NAME	_T("data/TEXTURE/GUI/titleGUI.png")				//タイトルGUIのテクスチャ
#define TITLE_COPYRIGHT_NAME	_T("data/TEXTURE/bg/copyright.png")			//コピーライトのテクスチャ

#define TITLE_TEXTURE_SIZE_X				(SCREEN_WIDTH / 2)		// テクスチャサイズX
#define TITLE_TEXTURE_SIZE_Y				(SCREEN_HEIGHT / 2)		// テクスチャサイズY

#define TITLE_INITPOS_X						(SCREEN_CENTER_X)		//タイトルの初期X座標
#define TITLE_INITPOS_Y						(SCREEN_CENTER_Y)		//タイトルの初期Y座標

#define TITLE_LOGO_INITPOS_X				(SCREEN_CENTER_X)		//タイトル背景の初期座標
#define TITLE_LOGO_INITPOS_Y				(SCREEN_CENTER_Y-150)	//タイトル背景の初期座標
#define TITLE_LOGO_SIZE_X					(350.0f)				//タイトルロゴの初期座標
#define TITLE_LOGO_SIZE_Y					(350.0f)				//タイトルロゴの初期座標

#define TITLE_DIFFICULTY_DIVIDE_Y			(4)						//難易度選択のテクスチャ分割数
#define TITLE_DIFFICULTY_INITPOS_X			(SCREEN_CENTER_X)		//難易度選択の初期座標
#define TITLE_DIFFICULTY_INITPOS_Y			(820)					//難易度選択の初期座標
#define TITLE_DIFFICULTY_SIZE_X				(300)					//難易度選択のテクスチャサイズ
#define TITLE_DIFFICULTY_SIZE_Y				(100)					//難易度選択のテクスチャサイズ

#define TITLE_GEARCENTER_SIZE_X				(500)					//大きい歯車のテクスチャサイズ
#define TITLE_GEARCENTER_SIZE_Y				(500)					//大きい歯車のテクスチャサイズ

#define TITLE_GUITEX_SIZE_X					(400)					//GUIテクスチャのサイズ
#define TITLE_GUITEX_SIZE_Y					(200)					//GUIテクスチャのサイズ
#define TITLE_GUITEX_DIVIDE_Y				(2)						//GUIテクスチャのY分割数

#define TITLE_COPYRIGHT_SIZE_X				(400)					//コピーライトのテクスチャサイズ
#define TITLE_COPYRIGHT_SIZE_Y				(100)					//コピーライトのテクスチャサイズ
#define TITLE_COPYRIGHT_INITPOS_X			(880)					//コピーライトの初期座標
#define TITLE_COPYRIGHT_INITPOS_Y			(870)					//コピーライトの初期座標
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	int						state;
	int						cntFrame;

	D3DXVECTOR3				pos;						// ポリゴンの座標
	D3DXVECTOR3				rot;						// ポリゴンの回転量

	float					radius;						// ポリゴンの半径
	float					angle;						// ポリゴンの角度

	float					radiusLogo;					//ロゴのポリゴン半径
	float					angleLogo;					//ロゴのポリゴン角度

	float					radiusDifficulty;			//難易度選択のポリゴン半径
	float					angleDifficulty;			//難易度選択のポリゴン角度
	int						difficulty;					//難易度

	float					radiusGearSmall;			//小さい歯車のポリゴン半径
	float					angleGearSmall;				//小さい歯車のポリゴン角度

	float					radiusGearBig;				//大きい歯車のポリゴン半径
	float					angleGearBig;				//大きい歯車のポリゴン角度

	float					radiusGUI;					//GUIのポリゴン半径
	float					angleGUI;					//GUIのポリゴン角度
}TITLE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTitle(int num);							//タイトル初期化処理
void UninitTitle(void);								//タイトル終了処理
void UpdateTitle(void);								//タイトル更新処理
void DrawTitle(void);								//タイトル描画処理
TITLE *GetTitleAdr(void);						//タイトルアドレス取得関数

#endif
