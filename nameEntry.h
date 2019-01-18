//=============================================================================
//
// ネームエントリーヘッダ [NameEntry.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _NAMEENTRY_H_
#define _NAMEENTRY_H_


// マクロ定義
#define NAMEENTRY_TEXTURE_NAME	_T("data/TEXTURE/GUI/nameEntryBG.png")			// ネームエントリーのテクスチャ
#define NAMEENTRY_CURSORTEX_NAME _T("data/TEXTURE/GUI/nameEntryCursor.png")		//カーソルのテクスチャ
#define NAMEENTRY_GUIDETEX_NAME _T("data/TEXTURE/GUI/nameEntryGuide.png")		//ガイドのテクスチャ

#define NAMEENTRY_CHARTEX_SIZE_X		(30.0f)					//文字セットのテクスチャサイズ
#define NAMEENTRY_CHARTEX_SIZE_Y		(30.0f)					

#define NAMEENTRY_NAMETEX_SIZE_X		(20.0f)					//プレイヤー名のテクスチャサイズ
#define NAMEENTRY_NAMETEX_SIZE_Y		(20.0f)

#define NAMEENTRY_SCORETEX_SIZE_X		(20.0f)					//スコアのテクスチャサイズ
#define NAMEENTRY_SCORETEX_SIZE_Y		(20.0f)

#define NAMEENTRY_CURSORTEX_SIZE_X		(NAMEENTRY_CHARTEX_SIZE_X*1.5f)	//カーソルのテクスチャサイズ
#define NAMEENTRY_CURSORTEX_SIZE_Y		(NAMEENTRY_CHARTEX_SIZE_Y*1.5f)
#define NAMEENTRY_CURSORTEX_DIVIDE_X	(2)								//カーソルのテクスチャ分割数	
#define NAMEENTRY_CURSORTEX_DIVIDE_Y	(2)
#define NAMEENTRY_CURSORTEX_ANIMAX		(NAMEENTRY_CURSORTEX_DIVIDE_X*NAMEENTRY_CURSORTEX_DIVIDE_X) //カーソルのアニメーション最大数

#define NAMEENTRY_GUIDETEX_SIZE_X		(1280)					//説明テキストのテクスチャサイズ
#define NAMEENTRY_GUIDETEX_SIZE_Y		(180)
#define NAMEENTRY_GUIDETEX_DIVIDE_Y		(2)						//説明テキストの分割数

#define NAMEENTRY_CAHR_MAX				(5)						//入力できる文字の最大数
#define NAMEENTRY_CHAR_INITPOS_X		(150.0f)				//文字セットの初期座標
#define NAMEENTRY_CHAR_INITPOS_Y		(830)
#define NAMEENTRY_CHAR_OFFSETPOS_X		(NAMEENTRY_CHARTEX_SIZE_X*2)	//文字セットのオフセット値
#define NAMEENTRY_CHAR_OFFSETPOS_Y		(NAMEENTRY_CHARTEX_SIZE_Y*2)

#define NAMEENTRY_NAME_INITPOS_X		(805.0f)				//ネームの初期座標
#define NAMEENTRY_NAME_INITPOS_Y		(740.0f)
#define NAMEENTRY_NAME_OFFSETPOS		(42.0f)					//ネームのオフセット値

#define NAMEENTRY_HIGHSCORE_INITPOS_X	(650.0f)				//ハイスコアの初期座標
#define NAMEENTRY_HIGHSCORE_INITPOS_Y	(170.0f)
#define NAMEENTRY_HIGHSCORE_OFFSET_X	(-30.0f)				//ハイスコアのオフセット値
#define NAMEENTRY_HIGHSCORE_OFFSET_Y	(55.0f)

#define NAMEENTRY_SCORENAME_INITPOS_X	(800.0f)				//ハイスコアのプレイヤー名の初期座標
#define NAMEENTRY_SCORENAME_INITPOS_Y	(NAMEENTRY_HIGHSCORE_INITPOS_Y)

#define NAMEENTRY_CURRENTSCORE_INITPOS_X (650)					//今回のスコア表示の初期座標
#define NAMEENTRY_CURRENTSCORE_INITPOS_Y (NAMEENTRY_NAME_INITPOS_Y)

#define NAMEENTRY_GUIDE_INITPOS_X		(SCREEN_CENTER_X)		//説明テキストの初期座標
#define NAMEENTRY_GUIDE_INITPOS_Y		(780)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	int						state;						//状態

	int						cntFrame;					//カウントフレーム
	D3DXVECTOR3				pos;						// ポリゴンの座標
	D3DXVECTOR3				rot;						// ポリゴンの回転量

	float					radiusChar;					// ポリゴンの半径
	float					angleChar;					// ポリゴンの角度
	int						selectedChar;				//今選ばれている文字
	int						nameBuff[NAMEENTRY_CAHR_MAX];	//プレイヤー名
	int						wordCnt;					//現在のプレイヤー名の文字数

	float					radiusCursor;				//カーソルのポリゴン半径
	float					angleCursor;				//カーソルのポリゴン角度

	float					radiusName;					//ネームのポリゴン半径
	float					angleName;					//ネームのポリゴン角度

	float					radiusScore;				//スコアのポリゴン半径
	float					angleScore;					//スコアのポリゴン角度
}NAMEENTRY;

//ネームエントリー画面の状態列挙子
enum NAMEENTRY_STATE
{
	NAMEENTRY_INPUT,
	NAMEENTRY_WAIT,
	NAMEENTRY_STATEMAX
};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitNameEntry(int num);							//ネームエントリー初期化処理
void UninitNameEntry(void);								//ネームエントリー終了処理
void UpdateNameEntry(void);								//ネームエントリー更新処理
void DrawNameEntry(void);								//ネームエントリー描画処理
NAMEENTRY *GetNameEntryAdr(void);						//ネームエントリーアドレス取得関数

#endif
