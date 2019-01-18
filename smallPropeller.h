//=============================================================================
//
// スモールプロペラヘッダ [SmallPropeller.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _SMALLPROPELLER_H_
#define _SMALLPROPELLER_H_


// マクロ定義
#define SMALLPROPELLER_TEXTURE_NAME	_T("data/TEXTURE/Enemy/sPropela.png")			// スモールプロペラのテクスチャ
#define SMALLPROPELLER_SHADOWTEX_NAME _T("data/TEXTURE/Shadow/sPropela.png")		//スモールプロペラのテクスチャ（影用）

#define SMALLPROPELLER_TEXTURE_NUM		(2)											//スモールプロペラのテクスチャの数

#define SMALLPROPELLER_TEXTURE_SIZE_X					(56)						// テクスチャサイズX
#define SMALLPROPELLER_TEXTURE_SIZE_Y					(56)						// テクスチャサイズY

#define SMALLPROPELLER_TEXTURE_DIVIDE_X					(2)							// アニメパターンのテクスチャ内分割数（X)
#define SMALLPROPELLER_TEXTURE_DIVIDE_Y					(2)							// アニメパターンのテクスチャ内分割数（Y)
#define SMALLPROPELLER_ANIM_NUM			(SMALLPROPELLER_TEXTURE_DIVIDE_X*SMALLPROPELLER_TEXTURE_DIVIDE_Y)	// アニメーションパターン数
#define SMALLPROPELLER_TIME_ANIMATION					(2)							// アニメーションの切り替わるカウント

#define SMALLPROPELLER_MAX								(32)						//スモールプロペラの最大数

#define SMALLPROPELLER_INITPOS_X						(-100)						//スモールプロペラの初期X座標
#define SMALLPROPELLER_INITPOS_Y						(-100)						//スモールプロペラの初期Y座標

//スモールプロペラのバウンディングボックスの定義
#define SMALLPROPELLER_BB_TOP							(0.0f)
#define SMALLPROPELLER_BB_LEFT							(0.0f)
#define SMALLPROPELLER_BB_WIDTH							(55.0f)
#define SMALLPROPELLER_BB_HIGHT							(55.0f)

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

	D3DXVECTOR3				targetPos;						//目標座標

	int						hp;								//HP
	int						state;							//行動状態

	int						attackNum;						//攻撃した回数
	int						countFrame;						//フレームのカウント数

	float					shotPosRadius;					//バレット発射位置の半径
	float					shotPosAngle;					//バレット発射位置の角度
}SMALLPROPELLER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSmallPropeller(int num);						//スモールプロペラ初期化処理
void UninitSmallPropeller(void);							//スモールプロペラ終了処理
void UpdateSmallPropeller(void);							//スモールプロペラ更新処理
void DrawSmallPropeller(void);								//スモールプロペラ描画処理
SMALLPROPELLER *GetSmallPropellerAdr(int pno);				//スモールプロペラアドレス取得関数
void SetSmallPropeller(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos); //スモールプロペラを出現させる関数

#endif
