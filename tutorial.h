//=============================================================================
//
// チュートリアルヘッダ [Tutorial.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

// マクロ定義
#define TUTORIAL_TEXTURE_NAME	_T("data/TEXTURE/GUI/tutorial.png")		//チュートリアルのテクスチャ
#define TUTORIAL_CHARTEX_NAME	_T("data/TEXTURE/GUI/tutorialChar.png")	//TUTORIAL文字テクスチャ
#define TUTORIAL_BGTEX_NAME		_T("data/TEXTURE/bg/tutorialBG.png")	//チュートリアル背景のテクスチャ
#define TUTORIAL_SKIPTEX_NAME		_T("data/TEXTURE/GUI/skip.png")		//スキップ文字のテクスチャ
#define TUTORIAL_SKIPPADTEX_NAME	_T("data/TEXTURE/GUI/skipPad.png")	//パッド用のスキップ文字
#define TUTORIAL_PADTEX_NAME	_T("data/TEXTURE/GUI/tutorialPad.png")	//パッド用チュートリアルのテクスチャ

#define TUTORIAL_DIVIDE_X			(1)					//チュートリアルテクスチャの分割数X
#define TUTORIAL_DIVIDE_Y			(5)					//チュートリアルのテクスチャ分割数Y

#define TUTORIAL_TEXTURE_SIZE_X		(390)				//チュートリアルテクスチャサイズX
#define TUTORIAL_TEXTURE_SIZE_Y		(130)				//チュートリアルテクスチャサイズY

#define TUTORIAL_INITPOS_X			(SCREEN_CENTER_X)	//チュートリアルのテクスチャ初期座標X
#define TUTORIAL_INITPOS_Y			(200)				//チュートリアルのテクスチャ初期座標Y

#define TUTORIAL_CHAR_INITPOS_X		(SCREEN_CENTER_X)	//TUTORIAL文字の初期座標
#define TUTORIAL_CHAR_INITPOS_Y		(50)				//TUTORIAL文字の初期座標
#define TUTORIAL_CHAR_SIZE_X		(120)				//TUTORIAL文字のテクスチャサイズ
#define TUTORIAL_CHAR_SIZE_Y		(40)				//TUTORIAL文字のテクスチャ

#define TUTORIAL_SKIPTEX_INITPOS_X	(630)				//スキップ文字の初期座標X
#define TUTORIAL_SKIPTEX_INITPOS_Y	(910)				//スキップ文字の初期座標Y
#define TUTORIAL_SKIPTEX_SIZE_X		(400)				//スキップ文字のテクスチャサイズX
#define TUTORIAL_SKIPTEX_SIZE_Y		(50)				//スキップ文字のテクスチャサイズY

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	int				cntFrame;
	int				tutorialIndex;

	float			angleGuide;
	float			radiusGuide;

	float			angleChar;
	float			radiusChar;

	float			bgOffset;
	int				guideState;
	float			guideAlpha;

	int				buttonCount;
}TUTORIAL;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTutorial(int num);							//チュートリアル初期化処理
void UninitTutorial(void);								//チュートリアル終了処理
void UpdateTutorial(void);								//チュートリアル更新処理
void DrawTutorial(void);								//チュートリアル描画処理
void DrawTutorialBG(void);								//チュートリアル背景描画処理
#endif
