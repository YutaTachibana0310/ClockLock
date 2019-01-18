//=============================================================================
//
// テストエネミーヘッダ [TestEnemy.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _TESTENEMY_H_
#define _TESTENEMY_H_

// マクロ定義
#define TESTENEMY_TEXTURE_NAME	_T("data/TEXTURE/Enemy/wideEnemy.png")	// テクスチャ

#define TESTENEMY_TEXTURE_SIZE_X					(140 / 2)			// テクスチャサイズX
#define TESTENEMY_TEXTURE_SIZE_Y					(120 / 2)			// テクスチャサイズY

#define TESTENEMY_TEXTURE_DIVIDE_X				(1)						// アニメパターンのテクスチャ内分割数（X)
#define TESTENEMY_TEXTURE_DIVIDE_Y				(1)						// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_TESTENEMY_NUM			(TESTENEMY_TEXTURE_DIVIDE_X*TESTENEMY_TEXTURE_DIVIDE_Y)	// アニメーションパターン数
#define TIMETESTENEMY_ANIMATION					(4)						// アニメーションの切り替わるカウント

#define TESTENEMY_MAX						(56)					//最大数（メインショット）

#define TESTENEMY_INITPOS_X						(-100)					//初期X座標
#define TESTENEMY_INITPOS_Y						(-100)					//初期Y座標

//バウンディングボックスの定義
#define TESTENEMY_BB_TOP						(0.0f)
#define TESTENEMY_BB_LEFT						(0.0f)
#define TESTENEMY_BB_WIDTH						(60.0f)
#define TESTENEMY_BB_HIGHT						(50.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	bool					active;							//使用or未使用の判定

	D3DXVECTOR3				pos;							// ポリゴンの座標
	D3DXVECTOR3				rot;							// ポリゴンの回転量
	int						countAnim;						// アニメーションカウント
	int						patternAnim;					// アニメーションパターンナンバー

	float					radius;							// ポリゴンの半径
	float					angle;							// ポリゴンの角度

	D3DXVECTOR3				targetPos;
	int state;
	int interbal;
	int attackNum;

	int hp = 100;
}TESTENEMY;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTestEnemy(int num);
void UninitTestEnemy(void);
void UpdateTestEnemy(void);
void DrawTestEnemy(void);
TESTENEMY *GetTestEnemyAdr(int pno);

#endif
