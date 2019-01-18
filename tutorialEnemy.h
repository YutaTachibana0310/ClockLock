//=============================================================================
//
// チュートリアルエネミーヘッダ [TutorialEnemy.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _TUTORIALENEMY_H_
#define _TUTORIALENEMY_H_


// マクロ定義
#define TUTORIALENEMY_TEXTURE_NAME	_T("data/TEXTURE/Enemy/TutorialEnemy.png")	// チュートリアルエネミーのテクスチャ

#define TUTORIALENEMY_TEXTURE_SIZE_X				(154)					// テクスチャサイズX
#define TUTORIALENEMY_TEXTURE_SIZE_Y				(70)					// テクスチャサイズY

#define TUTORIALENEMY_TEXTURE_DIVIDE_X				(1)						// アニメパターンのテクスチャ内分割数（X)
#define TUTORIALENEMY_TEXTURE_DIVIDE_Y				(1)						// アニメパターンのテクスチャ内分割数（Y)
#define TUTORIALENEMY_ANIM_NUM			(TUTORIALENEMY_TEXTURE_DIVIDE_X*TUTORIALENEMY_TEXTURE_DIVIDE_Y)	// アニメーションパターン数
#define TUTORIALENEMY_TIME_ANIMATION				(4)						// アニメーションの切り替わるカウント

#define TUTORIALENEMY_MAX							(2)						//チュートリアルエネミーの最大数

//チュートリアルエネミーのバウンディングボックスの定義
#define TUTORIALENEMY_BB_TOP						(-10.0f)
#define TUTORIALENEMY_BB_LEFT						(0.0f)
#define TUTORIALENEMY_BB_WIDTH						(140.0f)
#define TUTORIALENEMY_BB_HIGHT						(20.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	bool					active;						//使用or未使用の判定

	D3DXVECTOR3				pos;						// ポリゴンの座標
	D3DXVECTOR3				rot;						// ポリゴンの回転量
	int						countAnim;					// アニメーションカウント
	int						patternAnim;				// アニメーションパターンナンバー
	int						id;

	D3DXVECTOR3				targetPos;					// 目標座標
	int						state;						// 状態
	int						hp;							// HP

}TUTORIALENEMY;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTutorialEnemy(int num);							//チュートリアルエネミー初期化処理
void UninitTutorialEnemy(void);								//チュートリアルエネミー終了処理
void UpdateTutorialEnemy(void);								//チュートリアルエネミー更新処理
void DrawTutorialEnemy(void);								//チュートリアルエネミー描画処理
TUTORIALENEMY *GetTutorialEnemyAdr(int pno);				//チュートリアルエネミーアドレス取得関数
void SetTutorialEnemy(void);								//チュートリアルエネミーセット関数
void DisableTutorialEnemy(void);							//すべてのチュートリアルエネミーを非アクティブにする関数
#endif
